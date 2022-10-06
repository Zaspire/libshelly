#ifndef SHELLY_AVL_TREE_FDSHVCX43543
#define SHELLY_AVL_TREE_FDSHVCX43543

#include <cassert>
#include <memory>

namespace shelly {

template<typename key, typename value>
class AVLTree {
public:
  AVLTree(): _size(0) {
  }
  void Insert(const key k, const value v) {
    _size++;
    if (_root) {
      _root->Insert(k, v);
    } else {
      _root = std::unique_ptr<Node>(new Node(k, v));
    }
  }
  int height() {
    if (_root) {
      return _root->_height;
    }
    return 0;
  }
  size_t Size() {
    return _size;
  }
  bool Erase(const key k) {
    if (!_root) {
      return false;
    }
    bool b = _root->Erase(_root, k);
    if (b) {
      _size--;
    }
    return b;
  }
  std::pair<value, bool> Find(const key k) {
    if (!_root)
      return std::make_pair(value(), false);
    Node *n = _root->Find(k);
    if (n) {
      return std::make_pair(n->_value, true);
    }
    return std::make_pair(value(), false);
  }
  value& operator[](const key k) {
    if (_root) {
      Node *n = _root->Find(k);
      if (n) {
        return n->_value;
      }
    }
    Insert(k, value());

    return _root->Find(k)->_value;
  }
private:
  class Node {
  public:
    Node(const key k, const value v): _key(k), _value(v), _height(1) {}
    void Insert(const key k, const value v) {
      assert(k != _key);
      if (k < _key) {
        InsertToNode(_l, k, v);
        UpdateHeight();
      } else {
        InsertToNode(_r, k, v);
        UpdateHeight();
      }
      Balance();
    }
    std::unique_ptr<Node>& BiggestNode(std::unique_ptr<Node> &self) {
      if (_r) {
        return _r->BiggestNode(_r);
      }
      return self;
    }
    void BalanceRR() {
      if (_r) {
        _r->BalanceRR();
      }
      UpdateHeight();
      Balance();
    }
    static void RealErase(std::unique_ptr<Node> &self) {
      if (!self->_l && !self->_r) {
        self.reset();
        return;
      }
      if (self->_l && !self->_r) {
        self->_value = self->_l->_value;
        self->_key = self->_l->_key;

        self->_r = std::move(self->_l->_r);
        self->_l = std::move(self->_l->_l);

        self->UpdateHeight();
        self->Balance();
        return;
      }
      if (self->_r && !self->_l) {
        self->_value = self->_r->_value;
        self->_key = self->_r->_key;

        self->_l = std::move(self->_r->_l);
        self->_r = std::move(self->_r->_r);
        self->UpdateHeight();
        self->Balance();
        return;
      }
      std::unique_ptr<Node> &bn = self->_l->BiggestNode(self->_l);
      std::swap(bn->_value, self->_value);
      std::swap(bn->_key, self->_key);
      RealErase(bn);
      if (self->_l) {
        self->_l->BalanceRR();
      }
      self->UpdateHeight();
      self->Balance();
    }
    bool Erase(std::unique_ptr<Node> &self, key k) {
      if (_key == k) {
        RealErase(self);
        return true;
      }
      if (_key > k) {
        if (!_l) {
          return false;
        }
        bool b = _l->Erase(_l, k);
        UpdateHeight();
        Balance();
        return b;
      }
      if (!_r) {
        return false;
      }
      bool b = _r->Erase(_r, k);
      UpdateHeight();
      Balance();
      return b;
    }
    Node* Find(const key k) {
      if (_key == k) {
        return this;
      }
      if (_key > k) {
        if (!_l) {
          return nullptr;
        }
        return _l->Find(k);
      }
      if (!_r) {
        return nullptr;
      }
      return _r->Find(k);
    }
    void UpdateHeight() {
      int l = 0, r = 0;
      if (_l) {
        l = _l->_height;
      }
      if (_r) {
        r = _r->_height;
      }
      _height = std::max(l + 1, r + 1);
    }
    void RotateRight() {
      std::unique_ptr<Node> a = std::move(_l->_l), b = std::move(_l->_r);
      std::unique_ptr<Node> l = std::move(_l), r = std::move(_r);

      std::swap(l->_value, _value);
      std::swap(l->_key, _key);

      l->_l = std::move(b);
      l->_r = std::move(r);
      l->UpdateHeight();

      _l = std::move(a);
      _r = std::move(l);

      UpdateHeight();
    }
    void RotateLeft() {
      std::unique_ptr<Node> b = std::move(_r->_l), c = std::move(_r->_r);
      std::unique_ptr<Node> l = std::move(_l), r = std::move(_r);

      std::swap(r->_value, _value);
      std::swap(r->_key, _key);

      r->_l = std::move(l);
      r->_r = std::move(b);
      r->UpdateHeight();

      _r = std::move(c);
      _l = std::move(r);

      UpdateHeight();
    }
    void Balance() {
      if (std::abs(BalanceFactor()) <= 1) {
        return;
      }
      assert(std::abs(BalanceFactor()) <= 2);
      if (BalanceFactor() == 2) {
        if (_l->BalanceFactor() == -1) {
          _l->RotateLeft();
        }
        RotateRight();
      } else {
        if (_r && _r->BalanceFactor() == 1) {
          _r->RotateRight();
        }
        RotateLeft();
      }
    }
    int BalanceFactor() {
      int l = 0, r = 0;
      if (_l) {
        l = _l->_height;
      }
      if (_r) {
        r = _r->_height;
      }
      return l - r;
    }

    key _key;
    value _value;
    int _height;
    std::unique_ptr<Node> _l, _r;

    void InsertToNode(std::unique_ptr<Node> &node, key k, value v) {
      if (node) {
        node->Insert(k, v);
      } else {
        node = std::unique_ptr<Node>(new Node(k, v));
      }
    }
  };

  std::unique_ptr<Node> _root;
  size_t _size;
};

}

#endif

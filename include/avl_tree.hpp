#ifndef SHELLY_AVL_TREE_FDSHVCX43543
#define SHELLY_AVL_TREE_FDSHVCX43543

#include "common.hpp"

namespace shelly {

template<typename key, typename value>
class AVLTree {
public:
  AVLTree(): _size(0) {
  }
  void Insert(const key k, const value v) {
    _size++;
    if (!_root.get()) {
      _root = std::make_shared<Node>(k, v);
    } else {
      _root->Insert(k, v);
    }
  }
  int height() {
    if (_root.get())
      return _root->_height;
    return 0;
  }
  size_t Size() {
    return _size;
  }
  bool Erase(const key k) {
    if (!_root.get())
      return false;
    bool b = _root->Erase(_root, k);
    if (b)
      _size--;
    return b;
  }
  std::pair<value, bool> Find(const key k) {
    if (!_root.get())
      return std::make_pair(value(), false);
    Node *n = _root->Find(k);
    if (n)
      return std::make_pair(n->_value, true);
    return std::make_pair(value(), false);
  }
  value& operator[](const key k) {
    if (_root.get()) {
      Node *n = _root->Find(k);
      if (n)
        return n->_value;
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
        _height = std::max(_height, _l->_height + 1);
      } else {
        InsertToNode(_r, k, v);
        _height = std::max(_height, _r->_height + 1);
      }
      Balance();
    }
    std::shared_ptr<Node>& BiggestNode(std::shared_ptr<Node> &self) {
      if (_r.get())
        return _r->BiggestNode(_r);
      return self;
    }
    void BalanceRR() {
      if (_r.get()) {
        _r->BalanceRR();
      }
      UpdateHeight();
      Balance();
    }
    static void RealErase(std::shared_ptr<Node> &self) {
      if (!self->_l.get() && !self->_r.get()) {
        self.reset();
        return;
      }
      if (self->_l.get() && !self->_r.get()) {
        self->_value = self->_l->_value;
        self->_key = self->_l->_key;

        self->_r = self->_l->_r;
        self->_l = self->_l->_l;
        self->UpdateHeight();
        self->Balance();
        return;
      }
      if (self->_r.get() && !self->_l.get()) {
        self->_value = self->_r->_value;
        self->_key = self->_r->_key;

        self->_l = self->_r->_l;
        self->_r = self->_r->_r;
        self->UpdateHeight();
        self->Balance();
        return;
      }
      std::shared_ptr<Node> &bn = self->_l->BiggestNode(self->_l);
      std::swap(bn->_value, self->_value);
      std::swap(bn->_key, self->_key);
      RealErase(bn);
      if (self->_l.get())
        self->_l->BalanceRR();
      self->UpdateHeight();
      self->Balance();
    }
    bool Erase(std::shared_ptr<Node> &self, key k) {
      if (_key == k) {
        RealErase(self);
        return true;
      }
      if (_key > k) {
        if (!_l.get())
          return false;
        bool b = _l->Erase(_l, k);
        UpdateHeight();
        Balance();
        return b;
      }
      if (!_r.get())
        return false;
      bool b = _r->Erase(_r, k);
      UpdateHeight();
      Balance();
      return b;
    }
    Node* Find(const key k) {
      if (_key == k)
        return this;
      if (_key > k) {
        if (!_l.get())
          return nullptr;
        return _l->Find(k);
      }
      if (!_r.get())
        return nullptr;
      return _r->Find(k);
    }
    void UpdateHeight() {
      int l = 0, r = 0;
      if (_l.get())
        l = _l->_height;
      if (_r.get())
        r = _r->_height;
      _height = std::max(l + 1, r + 1);
    }
    void RotateRight() {
      auto a = _l->_l, b = _l->_r, l = _l, r = _r;
      std::swap(_l->_value, _value);
      std::swap(_l->_key, _key);

      _l = a;
      _r = l;

      l->_l = b;
      l->_r = r;

      l->UpdateHeight();
      UpdateHeight();
    }
    void RotateLeft() {
      auto b = _r->_l, c = _r->_r, l = _l, r = _r;
      std::swap(_r->_value, _value);
      std::swap(_r->_key, _key);

      _r = c;
      _l = r;
      r->_l = l;
      r->_r = b;

      r->UpdateHeight();
      UpdateHeight();
    }
    void Balance() {
      if (std::abs(BalanceFactor()) <= 1)
        return;
      assert(std::abs(BalanceFactor()) <= 2);
      if (BalanceFactor() == 2) {
        if (_l->BalanceFactor() == -1) {
          _l->RotateLeft();
        }
        RotateRight();
      } else {
        if (_r.get() && _r->BalanceFactor() == 1) {
          _r->RotateRight();
        }
        RotateLeft();
      }
    }
    int BalanceFactor() {
      int l = 0, r = 0;
      if (_l.get())
        l = _l->_height;
      if (_r.get())
        r = _r->_height;
      return l - r;
    }
    key _key;
    value _value;
    int _height;
    std::shared_ptr<Node> _l, _r;
  private:
    void InsertToNode(std::shared_ptr<Node> &node, key k, value v) {
      if (node.get())
        node->Insert(k, v);
      else
        node = std::make_shared<Node>(k, v);
    }
  };
  std::shared_ptr<Node> _root;
  size_t _size;
};

}

#endif

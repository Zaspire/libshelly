#ifndef SHELLY_AVL_TREE_FDSHVCX43543
#define SHELLY_AVL_TREE_FDSHVCX43543

#include "common.hpp"

namespace shelly {
    template<typename key, typename value>
    class AVLTree {
    public:
        AVLTree() {}
        void insert(key k, value v) {
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
        value Find(key k) {
            return _root->Find(k);
        }
    private:
        class Node {
        public:
            Node(key k, value v): _key(k), _value(v), _height(1) {}
            void Insert(key k, value v) {
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
            value Find(key k) {
                if (_key == k)
                    return _value;
                if (_key > k) {
                    assert(_l.get());
                    return _l->Find(k);
                }
                assert(_r.get());
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
    };

}

#endif

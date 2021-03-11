//
// Created by 王波 on 2020/12/8.
//
#if TREE_TYPE == 1
#define TREE rb_tree
#elif TREE_TYPE == 2
#define TREE interval_tree
#endif

#ifndef PROJECT3_RB_TREE_HPP
#define PROJECT3_RB_TREE_HPP

#include <algorithm>
#include <utility>
#include <optional>
#include <cassert>
#include <ostream>

namespace data_structure{
    //dam stupid apple clang doesn't support using enum in C++20!
    enum class color : bool { red = false, black = true };

    template<typename T>
    struct TREE{
        //node should be private, but for convenience not
        struct node{
#if TREE_TYPE !=2
            T key;
#else
            std::pair<T, T> key;
            T max = key.second;
#endif
            color color_ = color::black;
            node* left = this;
            node* right = this;
            node* p = this;

            //pos alias for get_position, p alias for position
            template<bool p>
            inline node*& pos(){ return p ? left : right; }
            template<bool p>
            inline const node*& pos() const { return p ? left : right; }
        };

        node *root = new node;
        node *nil{root};

    private:
        //left_rotate = rotate<true>
        template<bool p>
        void rotate(node* x){
            node* y = x->template pos<!p>();
            x->template pos<!p>() = y->template pos<p>();
            if (y->template pos<p>() != nil)
                y->template pos<p>()->p = x;
            y->p = x->p;
            if (x->p == nil)
                root = y;
            else if (x == x->p->left)
                x->p->left = y;
            else
                x->p->right = y;
            y->template pos<p>() = x;
            x->p = y;
#if TREE_TYPE == 2
            x->max = std::max({x->key.second, x->left->max, x->right->max});
            y->max = std::max({y->key.second, y->left->max, y->right->max});
#endif
        }

        //insert_fixup same as textbook
        inline void insert_fixup(node* z){
            while (z->p->color_ == color::red){
                 auto clause = [&, this]<bool p>(std::bool_constant<p> s){
                     node* y = z->p->p->template pos<!p>();
                     if (y->color_ == color::red) {
                         //case 1
                         z->p->color_ = color::black;
                         y->color_ = color::black;
                         z->p->p->color_ = color::red;
                         z = z->p->p;
                     } else {
                         if (z == z->p->template pos<!p>()) {
                             //case 2
                             z = z->p;
                             rotate<p>(z);
                         }
                         //case 3
                         z->p->color_ = color::black;
                         z->p->p->color_ = color::red;
                         rotate<!p>(z->p->p);
                     }
                 };
                 //clause with right and left exchanged
                z->p == z->p->p->left ? clause(std::true_type{}) : clause(std::false_type{});
            }
            root->color_ = color::black;
        }

        //insert same as textbook
        void insert(node* z){
            node* y = nil;
            auto x = root;
            while (x != nil){
#if TREE_TYPE == 2
                x->max = std::max(x->max, z->max);
#endif
                y = x;
                x = z->key < x->key ? x->left : x->right;
            }
            z->p = y;
            if (y == nil)
                root = z;
            else if (z->key < y->key)
                y->left = z;
            else
                y->right = z;
            z->color_ = color::red;
            insert_fixup(z);
        }

        //transplant same as textbook
        inline void transplant(node* u, node* v){
            if (u->p == nil)
                root = v;
            else if (u == u->p->left)
                u->p->left = v;
            else
                u->p->right = v;
            v->p = u->p;
        }

        //delete_fixup same as textbook
        inline void delete_fixup(node* x){
            while (x != root && x->color_ == color::black){
                auto clause = [&, this]<bool p>(std::bool_constant<p>) {
                    node* w = x->p->template pos<!p>();
                    if (w->color_ == color::red){
                        //case 1
                        w->color_ = color::black;
                        x->p->color_ = color::red;
                        rotate<p>(x->p);
                        w = x->p->template pos<!p>();
                    }
                    if (w->left->color_ == color::black && w->right->color_ == color::black){
                        //case 2
                        w->color_ = color::red;
                        x = x->p;
                    } else {
                        if (w->template pos<!p>()->color_ == color::black) {
                            //case 3
                            w->template pos<p>()->color_ = color::black;
                            w->color_ = color::red;
                            rotate<!p>(w);
                            w = x->p->template pos<!p>();
                        }
                        //case 4
                        w->color_ = x->p->color_;
                        x->p->color_ = color::black;
                        w->template pos<!p>()->color_ = color::black;
                        rotate<p>(x->p);
                        x = root;
                    }
                };
                //clause with right and left exchanged
                x == x->p->left ? clause(std::true_type{}) : clause(std::false_type{});
            }
            x->color_ = color::black;
        }

        inline node* minimum(node* z){
            while (z->left != nil)
                z = z->left;
            return z;
        }

        //erase node
        auto erase(node* z){
            node *y = z, *x;
            auto y_original_color = y->color_;
            if (z->left == nil){
                //case 1
                x = z->right;
                transplant(z, z->right);
            } else if (z->right == nil){
                //case 2
                x = z->left;
                transplant(z, z->left);
            } else{
                //case 3, 4
                y = minimum(z->right);
                y_original_color = y->color_;
                x = y->right;
                if (y->p == z)
                    x->p = y;
                else {
                    transplant(y, y->right);
                    y->right = z->right;
                    y->right->p = y;
                }
                [[maybe_unused]] auto w = y->p;
                transplant(z, y);
#if TREE_TYPE == 2
                for (; w != y && w->max <= y->max; w = w->p)
                    w->max = std::max({w->key.second, w->left->max, w->right->max});
#endif
                y->left = z->left;
                y->left->p = y;
                y->color_ = z->color_;
            }
#if TREE_TYPE == 2
            for (auto w = y; w != nil && w->max <= z->max; w = w->p)
                w->max = std::max({w->key.second, w->left->max, w->right->max});
            auto max = z->max;
#endif
            if (y_original_color == color::black)
                delete_fixup(x);
            delete z;
#if TREE_TYPE == 2
            return max;
#endif
        }

        //print node
        void print(std::ostream& os, node* nd) const{
            if (nd != nil){
                print(os, nd->left);
#if TREE_TYPE == 1
                os << nd->key << " ";
#elif TREE_TYPE == 2
                os << nd->key.first << " " << nd->key.second << " " << nd->max << '\n';
                //in debug mode, check max status and algorithm accuracy
                assert(nd->max >= nd->left->max && nd->max >= nd->right->max);
#endif
                print(os, nd->right);
            }
        }

        //delete the node and nodes below
        void destroy(node* s){
            if (s != nil){
                destroy(s->left);
                destroy(s->right);
                delete s;
            }
        }

    public:
        //print tree
        friend std::ostream& operator<<(std::ostream& os, const TREE<T>& tree) {
            tree.print(os, tree.root);
            return os;
        }

        //insert value rather than node
        template<typename... Ts>
        inline void emplace(Ts&&... ts){
            insert(new node {.key{std::forward<Ts>(ts)...}, .color_{color::red},
                             .left{nil} ,.right{nil}, .p{nil}});
        }
#if TREE_TYPE == 2
        //if you do not like std::nullopt, use std::numeric_limits<std::size_t>::max()
        [[nodiscard]] auto interval_search (std::pair<T, T> i) const{
            if (i.first > i.second)
                throw std::range_error("not an interval");
            auto x = root;
            while (x != nil && ((x->key.first > i.first) && (x->key.second > i.second) ||
                    (x->key.first < i.first) && (x->key.second < i.second)))
                x = x->left != nil && x->left->max >= i.first ? x->left : x->right;
            return x == nil ? std::nullopt : std::optional{x->key};
        }
#endif
        //remove value rather than node
        auto remove(decltype(node::key) value){
            auto s = root;
            while (s != nil && s->key != value)
                s = value < s->key ? s->left : s->right;
            if (s == nil)
                throw std::range_error("non-exist value");
            return erase(s);
        }

        //deconstruct
        ~TREE(){
            destroy(root);
            delete nil;
        }
    };
}

#endif //PROJECT3_RB_TREE_HPP

#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insert_fix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void remove_fix(AVLNode<Key, Value>* n, int diff);
    void rightRotate(AVLNode<Key, Value>* g);
    void leftRotate(AVLNode<Key, Value>* g);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* temp3;
    if(this->root_==NULL){
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);;
        static_cast<AVLNode<Key, Value>*>(this->root_)->setBalance(0);
    }
    else{
        AVLNode<Key, Value> * temp = static_cast<AVLNode<Key, Value>*>(this->root_);
        while(temp!=NULL){
			if(temp->getKey()==new_item.first){
				temp->setValue(new_item.second);
                temp3=temp;
				return;
			}
			if(temp->getKey()>new_item.first){
				if(temp->getLeft()==NULL) {
					AVLNode<Key, Value> * temp2 = new AVLNode<Key, Value>(new_item.first, new_item.second, temp);
					temp->setLeft(temp2);
                    temp3 = temp2;
					break;
				}
				temp = temp->getLeft();
			}
			if(temp->getKey()<new_item.first){
				if(temp->getRight()==NULL) {
					AVLNode<Key, Value> * temp2 = new AVLNode<Key, Value>(new_item.first, new_item.second, temp);
					temp->setRight(temp2);
                    temp3 = temp2;
					break;
				}
				temp = temp->getRight();
			}
		} 
        temp3->setBalance(0);           
        if(temp3->getParent()->getBalance()==-1){
                temp3->getParent()->setBalance(0);
        }
        else if(temp3->getParent()->getBalance()==1){
            temp3->getParent()->setBalance(0);
        }
        else if(temp3->getParent()->getBalance()==0){
            if(temp3->getParent()->getLeft()==temp3) {
                temp3->getParent()->setBalance(-1);
                insert_fix(temp3->getParent(), temp3);
            }
            else {
                temp3->getParent()->setBalance(1);
                insert_fix(temp3->getParent(),temp3);
            }
        }
        
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n){
    //std::cout<<"entered"<<std::endl;
    //std::cout << p->getKey() << std::endl;
    if(p==NULL||p->getParent()==NULL||n==NULL) {
        return;
    }
    AVLNode<Key, Value> * g = p->getParent();
    //std::cout << p->getKey();
    if(p==g->getLeft()){
        g->updateBalance(-1);
        if(g->getBalance()==0) return;
        else if(g->getBalance()==-1) insert_fix(g,p);
        else if(g->getBalance()==-2) {
            if(n==p->getLeft()){
                rightRotate(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else{
                leftRotate(p);
                rightRotate(g);
                if(n->getBalance()==-1){
                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0);
                }
                else if(n->getBalance()==0){
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if(n->getBalance()==1){
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }
    else{
        g->updateBalance(1);
        if(g->getBalance()==0) return;
        else if(g->getBalance()==1) insert_fix(g,p);
        else if(g->getBalance()==2) {
            if(n==p->getRight()){
                leftRotate(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else{
                rightRotate(p);
                leftRotate(g);
                if(n->getBalance()==1){
                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0);
                }
                else if(n->getBalance()==0){
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if(n->getBalance()==-1){
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key,Value>* g){
        AVLNode<Key,Value>* temp = g->getLeft();
        if(g==this->root_){
					AVLNode<Key,Value>* temp2;
            if(g!=NULL&&g->getLeft()!=NULL) temp2 = g->getLeft()->getRight();
            if(g!=NULL&&g->getLeft()!=NULL) g->getLeft()->setRight(g);
            g->setLeft(temp2);
            if(temp2!=NULL){
                temp2->setParent(g);
            }
						if(temp!=NULL) temp->setParent(NULL);
            this->root_=temp;
        }
        else{
            if(g->getParent()->getLeft()==g){
                g->getParent()->setLeft(g->getLeft());
            }
            else{
                g->getParent()->setRight(g->getLeft());
            }
            if(g->getLeft()!=NULL) g->getLeft()->setParent(g->getParent());
            if(g!=NULL&&temp!=NULL) g->setLeft(temp->getRight());
            if(temp!=NULL){
                if(temp->getRight()!=NULL){
                    temp->getRight()->setParent(g);
                }   
            }
            if(temp!=NULL) temp->setRight(g);
        }
        g->setParent(temp);
}

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key,Value>* g){
        AVLNode<Key,Value>* temp = g->getRight();
        if(g==this->root_){
            AVLNode<Key,Value>* temp2 = g->getRight()->getLeft();
            temp->setLeft(g);
            g->setRight(temp2);
            if(temp2!=NULL){
                temp2->setParent(g);
            }
						temp->setParent(NULL);
            this->root_ = temp;
        }
        else{
            if(g->getParent()->getRight()==g){
                g->getParent()->setRight(g->getRight());
            }
            else{
                g->getParent()->setLeft(g->getRight());
            }
            //std::cout << "KKKKK" <<g->getRight()->getKey();
            //std::cout << g->getParent();
            if(g->getRight()!=NULL) temp->setParent(g->getParent());
            if(g!=NULL&&temp!=NULL) g->setRight(temp->getLeft());
            if(temp!=NULL){
                if(temp->getLeft()!=NULL){
                    temp->getLeft()->setParent(g);
                }
            }
            
            if(temp!=NULL) temp->setLeft(g);
        }
        g->setParent(temp);

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
        if(this->root_==NULL) return;
		AVLNode<Key, Value> * n = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
			AVLNode<Key,Value>* p;
				if(n!=NULL) p = n->getParent();
				else{
					return;
				}
				if(n->getLeft()!=NULL && n->getRight()!=NULL){
					nodeSwap(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(n)), n);
					p = n->getParent();
				}
				
				int diff;
        if(p!=NULL){
            if(n==p->getLeft()){
							
                diff=1;
                //p->updateBalance(1);
            } 
            else{
                diff=-1;
                //p->updateBalance(-1);
            }
        }
			
				if(n->getLeft()==NULL && n->getRight()==NULL){
					if(n==this->root_){
						delete n;
						n=NULL;
						this->root_ = NULL;
						return;
					} 
					if(n->getParent()->getLeft()==n){
						n->getParent()->setLeft(NULL);
						delete n;
						n=NULL;
					}
					else{
						n->getParent()->setRight(NULL);
						delete n;
						n=NULL;
					}
				}
				else if(n->getLeft()==NULL && n->getRight()!=NULL){
					if(n==this->root_){
						this->root_ = n->getRight();
						this->root_->setParent(NULL);
						delete n;
						n=NULL;
					}
					if(n!=NULL&&n->getParent()!=NULL){
if(n->getParent()->getLeft()==n){
						n->getParent()->setLeft(n->getRight());
						n->getRight()->setParent(n->getParent());
						delete n;
						n=NULL;
					}
					else{
						n->getParent()->setRight(n->getRight());
						n->getRight()->setParent(n->getParent());
						delete n;
						n=NULL;
					}
					}
					
				}
				else if(n->getLeft()!=NULL && n->getRight()==NULL){
					if(n==this->root_){
						this->root_ = this->root_->getLeft();
						this->root_->setParent(NULL);
						delete n;
						n=NULL;
					}
					if(n!=NULL&&n->getParent()!=NULL){
if(n->getParent()->getLeft()==n){
						n->getParent()->setLeft(n->getLeft());
						n->getLeft()->setParent(n->getParent());
						delete n;
						n=NULL;
					}
					else{
						 n->getParent()->setRight(n->getLeft());
						n->getLeft()->setParent(n->getParent());
						delete n;
						n=NULL;
					}
					}
					
				}
				 	
        remove_fix(p,diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove_fix(AVLNode<Key, Value>* n, int diff){
    if(n==NULL) return;
    AVLNode<Key, Value>* p = n->getParent();
    int ndiff;
    if(p!=NULL){
        if(n==p->getLeft()) ndiff=1;
        else ndiff=-1;
    }
    if(diff==-1){
        if(n->getBalance()+diff==-2){
            AVLNode<Key, Value>* c = n->getLeft();
            if(c->getBalance()==-1){
                rightRotate(n);
                n->setBalance(0);
                c->setBalance(0);
                remove_fix(p, ndiff);
            }
            else if(c->getBalance()==0){
                rightRotate(n);
                n->setBalance(-1);
                c->setBalance(1);
								return;
            }
            else if(c->getBalance()==1){
                AVLNode<Key, Value>* g = c->getRight();
                leftRotate(c);
                rightRotate(n);
								if(g!=NULL){
									if(g->getBalance()==1){
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                else if(g->getBalance()==0){
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if(g->getBalance()==-1){
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
								}
                
                remove_fix(p, ndiff);
            }
        }
        else if(n->getBalance()+diff==-1) {
					n->setBalance(-1);
					return;
				}
        else if(n->getBalance()+diff==0) {
            n->setBalance(0);
            remove_fix(p, ndiff);
        }
    }
    else if(diff==1){
        if(n->getBalance()+diff==2){
            AVLNode<Key, Value>* c = n->getRight();
						if(c!=NULL){
							if(c->getBalance()==1){
                leftRotate(n);
                n->setBalance(0);
                c->setBalance(0);
                remove_fix(p, ndiff);
            }
            else if(c->getBalance()==0){
                leftRotate(n);
                n->setBalance(1);
                c->setBalance(-1);
								return;
            }
            else if(c->getBalance()==-1){
                AVLNode<Key, Value>* g = c->getLeft();
                rightRotate(c);
                leftRotate(n);
                if(g->getBalance()==-1){
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                }
                else if(g->getBalance()==0){
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if(g->getBalance()==1){
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                remove_fix(p, ndiff);
            }
						}
            
        }
        else if(n->getBalance()+diff==1) {
					n->setBalance(1);
					return;
				}
        else if(n->getBalance()+diff==0) {
            n->setBalance(0);
            remove_fix(p, ndiff);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif

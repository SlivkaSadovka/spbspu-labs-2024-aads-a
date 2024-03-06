#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP
#include <cstddef>
#include <memory>
#include "node.hpp"
namespace strelyaev
{
  template< typename T >
  class ConstIterator
  {
    public:
      ConstIterator():
      node_(nullptr)
     {}

     ConstIterator(const ConstIterator& val):
      node_(val.node_)
     {}

     ConstIterator(Node< T >* val):
      node_(val)
     {}

     ~ConstIterator() = default;

      ConstIterator< T >& operator++()
      {
        if (node_ != nullptr)
        {
          node_ = node_->next_;
        }
        return *this;
      }

      ConstIterator< T >& operator++(int)
      {
        ConstIterator< T > result(*this);
        ++(*this);
        return result;
      }

      ConstIterator< T >& operator+(int n)
      {
       ConstIterator< T > temp(*this);
       for (int a = 0; a < n; a++)
       {
         ++temp;
       }
       return temp;
      }

      ConstIterator< T >& operator-(int n)
      {
       ConstIterator< T > temp(*this);
       for (int a = 0; a < n; a--)
       {
         --temp;
       }
       return temp;
      }

      T& operator*()
      {
        return node_->value_;
      }

      T* operator->()
      {
        return std::addressof(node_->value_);
      }

      bool operator==(const ConstIterator< T >& val) const
      {
        return node_ == val.node_;
      }

      bool operator!=(const ConstIterator< T >& val) const
      {
        return !(*this == val);
      }

     private:
       Node< T >* node_;
  };
}
#endif
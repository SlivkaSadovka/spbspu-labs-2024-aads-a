#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>

#include "node.hpp"

namespace zhalilov
{
  template < class T >
  class TwoThreeIterator: public std::iterator < std::bidirectional_iterator_tag, T >
  {
  public:
    ~TwoThreeIterator() = default;

    TwoThreeIterator &operator=(const TwoThreeIterator &) = default;

    TwoThreeIterator &operator++();
    TwoThreeIterator &operator--();
    TwoThreeIterator operator++(int);
    TwoThreeIterator operator--(int);

    T &operator*();
    T *operator->();
    const T &operator*() const;
    const T *operator->() const;

    bool operator==(const TwoThreeIterator < T > &) const;
    bool operator!=(const TwoThreeIterator < T > &) const;

  private:
    detail::Node < T > node_;
    bool isPtrToLeft_;

    explicit TwoThreeIterator(detail::Node < T > *node, bool isPtrToLeft = true);
    detail::Node < T > *findMin(detail::Node < T > *nodeFrom);
  };

  template < class T >
  TwoThreeIterator < T > &TwoThreeIterator < T >::operator++()
  {
    if (node_.type == detail::NodeType::Three)
    {
      if (isPtrToLeft_)
      {
        detail::Node < T > *minMid = findMin(node_.childs[1]);
        if (minMid == node_)
        {
          isPtrToLeft_ = false;
        }
        return *this;
      }
    }

    detail::Node < T > *minRight = findMin(node_.childs[2]);
    if (minRight == node_)
    {
      while (node_.parent->childs[2] == node_)
      {
        node_ = node_.parent;
      }
      isPtrToLeft_ = true;
    }
    else
    {
      node_ = minRight;
    }
    return *this;
  }

  template < class T >
  TwoThreeIterator < T > TwoThreeIterator < T >::operator++(int)
  {
    TwoThreeIterator temp(*this);
    operator++();
    return temp;
  }
}

#endif

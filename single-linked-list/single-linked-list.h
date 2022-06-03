#pragma once

#include <cstddef>
#include <utility>
#include <iterator>
#include <cassert>

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template<typename ValueType>
    class BasicIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }
        BasicIterator& operator++() noexcept {
            node_ = (node_) ? node_->next_node : nullptr;
            return *this;
        }
        BasicIterator operator++(int) noexcept {
            auto it_old = *this;
            ++(*this);
            return it_old;
        }
        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }
        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &(node_->value);
        }
    private:
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node): node_(node) {}
        Node* node_ = nullptr;
    public:

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }
        BasicIterator& operator=(const BasicIterator& rhs) = default;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(head_);
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(head_);
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_->next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator();
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_->next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator();
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_->next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator();
    }

    SingleLinkedList() : head_(new Node), size_(0) {}

    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    //template<typename Type>
    void PushFront(const Type& value) {
        head_->next_node = new Node(value, head_->next_node);
        ++size_;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        Node* new_node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = new_node;
        ++size_;
        return Iterator(new_node);
    }

    void PopFront() noexcept {
        if (size_ > 0) {
            Node* front_node = head_->next_node;
            head_->next_node = head_->next_node->next_node;
            --size_;
            delete front_node;
        }
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert((pos.node_ != nullptr) && (pos.node_->next_node != nullptr));
        Node* erase_node = pos.node_->next_node;
        pos.node_->next_node = pos.node_->next_node->next_node;
        delete erase_node;
        --size_;
        return Iterator(pos.node_->next_node);
    }

    void Clear() {
        while (size_ > 0) {
            PopFront();
        }
    }

    ~SingleLinkedList() {
        Clear();
        delete head_;
    }

    template<typename It>
    SingleLinkedList(It first, It second): SingleLinkedList() {
        for (auto it = first; it != second; ++it) {
            PushFront(*it);
        }
        Reverse();
    }

    SingleLinkedList(std::initializer_list<Type> values): SingleLinkedList(values.begin(), values.end()) {}

    SingleLinkedList(const SingleLinkedList& other): SingleLinkedList(other.begin(), other.end()) {}

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        SingleLinkedList list(rhs);
        swap(list);
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(size_, other.size_);
    }

    void Reverse() {
        Node* cur = head_->next_node;
        Node* prev = nullptr;
        while (cur) {
            Node* tmp = cur->next_node;
            cur->next_node = prev;
            prev = cur;
            cur = tmp;
        }
        head_->next_node = prev;
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node* head_;
    size_t size_ = 0;
};

template<typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template<typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template<typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return lhs < rhs || lhs == rhs;
}

template<typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template<typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs || lhs == rhs;
}
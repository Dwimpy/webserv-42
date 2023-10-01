#include <iostream>
#include <vector>
#include <utility>

template <typename KeyType, typename ValueType>
class OrderedMap {
public:
    typedef typename std::vector<std::pair<KeyType, ValueType> > KeyValueVector;

    void insert(const KeyType& key, const ValueType& value) {
        // Check if the key already exists
        for (typename KeyValueVector::iterator it = data.begin(); it != data.end(); ++it) {
            if (it->first == key) {
                it->second = value;
                return;
            }
        }

        // Key doesn't exist, add a new pair
        data.push_back(std::make_pair(key, value));
    }

    ValueType& operator[](const KeyType& key) {
        // Find the key and return its value
        for (typename KeyValueVector::iterator it = data.begin(); it != data.end(); ++it) {
            if (it->first == key) {
                return it->second;
            }
        }

        // Key not found, insert a new pair with a default-constructed value
        data.push_back(std::make_pair(key, ValueType()));
        return data.back().second;
    }

    typedef typename KeyValueVector::iterator iterator; // Define the iterator type
    iterator begin() {
        return data.begin();
    }

    iterator end() {
        return data.end();
    }

private:
    KeyValueVector data;
};

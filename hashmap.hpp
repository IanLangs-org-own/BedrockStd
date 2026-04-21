#pragma once

#include "arraylist.hpp"
#include "Result.hpp"
#include "reference.hpp"
#include "types.hpp"
#include "assert.hpp"

#include <functional>
#include <utility>

namespace bedrock {
namespace types {

template <typename T>
struct hash {
    size_t operator()(const T& v) const {
        return std::hash<T>{}(v);
    }
};

template <typename T>
concept Hashable = requires(const T& v) {
    { hash<T>{}(v) } -> std::convertible_to<size_t>;
};

} // namespace types
} // namespace bedrock


namespace bedrock::collections {

    template <bedrock::types::Hashable K, typename V>
    class HashMap {

        struct Item {
            K key;
            V value;
        };

        struct Bucket {
            ArrayList<Item> items;

            void insert_or_assign(const K& key, const V& value) {
                for (auto& item : items) {
                    if (item.key == key) {
                        item.value = value;
                        return;
                    }
                }
                items.append({key, value});
            }
        };

        ArrayList<Bucket> buckets;
        size_t bucket_count = 16;
        size_t size_value = 0;

        float load_factor() const {
            return bucket_count == 0 ? 0.f :
                (float)size_value / (float)bucket_count;
        }

        void rehash(size_t new_size) {
            ArrayList<Bucket> new_buckets;

            new_buckets.reserve(new_size);
            for (size_t i = 0; i < new_size; i++)
                new_buckets.emplace();

            for (auto& bucket : buckets) {
                for (auto& item : bucket.items) {
                    size_t index =
                        bedrock::types::hash<K>{}(item.key) % new_size;

                    new_buckets[index].insert_or_assign(
                        item.key,
                        item.value
                    );
                }
            }

            buckets = std::move(new_buckets);
            bucket_count = new_size;
        }

        void check_resize() {
            if (load_factor() >= 0.87f) {
                rehash(bucket_count * 2);
            }
        }

        public:

            HashMap() {
                buckets.reserve(bucket_count);
                for (size_t i = 0; i < bucket_count; i++)
                    buckets.emplace();
            }

            // =========================
            // INSERT
            // =========================

            void put(const K& key, const V& value) {
                check_resize();

                size_t index =
                    bedrock::types::hash<K>{}(key) % bucket_count;

                for (auto& item : buckets[index].items) {
                    if (item.key == key) {
                        item.value = value;
                        return;
                    }
                }

                buckets[index].items.append({key, value});
                size_value++;
            }

            // =========================
            // ACCESS (Result)
            // =========================

            bedrock::types::Result<
                bedrock::types::reference_t<V>, void
            >
            at(const K& key) {
                size_t index =
                    bedrock::types::hash<K>{}(key) % bucket_count;

                for (auto& item : buckets[index].items) {
                    if (item.key == key) {
                        return bedrock::ref(item.value);
                    }
                }

                return {};
            }

            bedrock::types::Result<
                bedrock::types::reference_t<const V>, void
            >
            at(const K& key) const {
                size_t index =
                    bedrock::types::hash<K>{}(key) % bucket_count;

                for (auto& item : buckets[index].items) {
                    if (item.key == key) {
                        return bedrock::ref<const V>(item.value);
                    }
                }

                return {};
            }

            // =========================
            // OPERATOR []
            // =========================

            V& operator[](const K& key) {
                check_resize();

                size_t index =
                    bedrock::types::hash<K>{}(key) % bucket_count;

                for (auto& item : buckets[index].items) {
                    if (item.key == key) {
                        return item.value;
                    }
                }

                buckets[index].items.append({key, V{}});
                size_value++;

                return buckets[index].items.back().value;
            }

            // =========================
            // INFO
            // =========================

            size_t size() const {
                return size_value;
            }

            size_t bucketCount() const {
                return bucket_count;
            }

            float loadFactor() const {
                return load_factor();
            }
    };

}
#pragma once

#include <vector>
#include <string_view>

namespace mdsm
{
    enum class Endianness
    {
        big, little
    };

    class Collection
    {
        public:
            Collection(const Endianness endianness);
            Collection(const Collection& collection);
            Collection(Collection&& collection);

            Collection& operator=(const Collection&  collection);
            Collection& operator=(      Collection&& collection);

            Collection& operator<<(const char* string);

            template <typename T>
            friend Collection& operator<<(Collection& destination, const T& source);

            template <typename T>
            friend Collection& operator<<(Collection&& destination, const T& source);

            template <typename T>
            friend Collection& operator>>(Collection& destination, T& source);

            template <typename T>
            T retrieve();

            template <typename T>
            friend T& operator<<(T& destination, const Collection& collection);

            template <typename T>
            friend T& operator>>(T& source, Collection& collection);

            Collection& clearOnSending  (bool value);
            Collection& clearOnReceiving(bool value);
            Collection& dropOnRetrieving(bool value);

            Collection& clear();

            size_t getSize() const;

            std::byte* getData();
            const std::byte* getData() const;

            void resize(size_t size);

        protected:
            std::vector<std::byte> data;

            bool clear_on_sending   {true};
            bool clear_on_receiving {true};
            bool drop_on_retrieving {true};

            void cropBytes(size_t bytes);
    };
}

#include "collection.ipp"
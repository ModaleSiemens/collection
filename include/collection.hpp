#pragma once

#include <vector>
#include <string_view>
#include <cstring>
#include <string>
#include <bit>
#include <algorithm>

namespace mdsm
{
    // Default Endianness: Big Endian
    class Collection
    {
        public:
            Collection() = default;
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

            bool clearsOnSending()   const;
            bool clearsOnReceiving() const;
            bool dropsOnRetrieving() const;

            Collection& clear();

            size_t getSize()     const;
            size_t getDataSize() const;

            bool isEmpty() const;

            std::byte* getData();
            const std::byte* getData() const;

            void resize(size_t size);

        private:
            std::vector<std::byte> data;

            bool clear_on_sending   {true};
            bool clear_on_receiving {true};
            bool drop_on_retrieving {true};

            void cropBytes(size_t bytes);

            template <typename T>
            static std::vector<std::byte> prepareDataForInserting(const T& source);
            
            template <typename T>
            static T prepareDataForExtracting(const std::byte* const data);            
    };
}

#include "collection.ipp"
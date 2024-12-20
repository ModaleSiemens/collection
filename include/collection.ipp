#pragma once

#include <string>
#include <iostream>
#include <cstring>
#include <string>

namespace mdsm
{
    template <typename T>
    inline T& operator<<(T&, const Collection&) = delete;

    template <typename T>
    inline T& operator>>(T&, Collection&) = delete;

    template <typename T>
    inline Collection& operator<<(Collection& collection, const T& source)
    {
        const auto previous_data_size {collection.getDataSize()};

        collection.resize(previous_data_size + sizeof(source));

        const auto proper_data {Collection::prepareDataForInserting(source)};

        std::memcpy(
            collection.getData() + previous_data_size,
            proper_data.data(),
            proper_data.size()
        );

        return collection;
    } 

    template <typename T>
    inline Collection& operator<<(Collection&& destination, const T& source)
    {
        return destination << source;
    }

    inline Collection& Collection::operator<<(const char* const string)
    {
        const auto previous_data_size {data.size()};

        data.resize(data.size() + std::strlen(string) + 1);

        std::memcpy(data.data() + previous_data_size, string, std::strlen(string) + 1);

        return *this;
    }

    template <>
    inline Collection& operator<<<std::string>(Collection& collection, const std::string& string)
    {
        const auto previous_data_size {collection.getDataSize()};

        collection.data.resize(collection.getDataSize() + string.length() + 1);

        std::memcpy(
            collection.getData() + previous_data_size,
            string.data(),
            string.length() + 1
        );

        return collection;
    }

    template <typename T>
    inline T Collection::retrieve()
    {
        T data;

        *this >> data;
        
        return data;
    }

    template <typename T>
    inline Collection& operator>>(Collection& collection, T& destination)
    {
        destination = std::move(Collection::prepareDataForExtracting<T>(collection.getData()));
            
        if(collection.dropsOnRetrieving())
        {
            collection.cropBytes(sizeof(destination));
        }

        return collection;
    }

    template <>
    inline Collection& operator>><std::string>(Collection& collection, std::string& string)
    {
        string = std::string{
            static_cast<char*>(static_cast<void*>(collection.getData()))
        };

        if(collection.dropsOnRetrieving())
        {
            collection.cropBytes(string.length() + 1);
        }

        return collection;
    }
}

namespace mdsm
{
    inline Collection::Collection(const Collection& collection)
    {
        this->operator=(collection);
    }

    inline Collection::Collection(Collection&& collection)
    {
        this->operator=(std::move(collection));
    }

    inline Collection& Collection::operator=(const Collection& collection)
    {
        data = collection.data;

        return *this;
    }

    inline Collection& Collection::operator=(Collection&& collection)
    {
        data = std::move(collection.data);

        return *this;
    }

    inline Collection& Collection::clearOnSending(const bool value)
    {
        clear_on_sending = value;

        return *this;
    }

    inline Collection& Collection::clearOnReceiving(const bool value)
    {
        clear_on_receiving = value;

        return *this;
    }

    inline Collection& Collection::dropOnRetrieving(const bool value)
    {
        drop_on_retrieving = value;

        return *this;
    }

    inline Collection& Collection::clear()
    {
        data.clear();

        return *this;
    }

    inline size_t Collection::getSize() const 
    {
        return data.size();
    }

    inline void Collection::cropBytes(const size_t bytes_count)
    {
        data.erase(data.begin(), data.begin() + bytes_count);
    }

    inline std::byte* Collection::getData()
    {
        return data.data();
    }

    inline const std::byte* Collection::getData() const
    {
        return data.data();
    }

    inline void Collection::resize(const size_t new_size)
    {
        data.resize(new_size);
    }
    
    inline bool Collection::isEmpty() const
    {
        return !getSize();
    }

    inline bool Collection::clearsOnSending() const
    {
        return clear_on_sending;
    }

    inline bool Collection::clearsOnReceiving() const
    {
        return clear_on_receiving;
    }

    inline bool Collection::dropsOnRetrieving() const
    {
        return drop_on_retrieving;
    }        

    inline size_t Collection::getDataSize() const 
    {
        return data.size();
    }

    template <typename T>
    inline std::vector<std::byte> Collection::prepareDataForInserting(const T& source)
    {
        std::vector<std::byte> data (sizeof(source));

        std::memcpy(data.data(), &source, sizeof(source));

        if(std::endian::native != std::endian::big)
        {
            std::ranges::reverse(data);
        }
        
        return data;
    }

    template <typename T>
    inline T Collection::prepareDataForExtracting(const std::byte* const t_data)
    {
        T object;

        if(std::endian::native != std::endian::big)
        {
            std::vector<std::byte> data {sizeof(T)};

            std::memcpy(data.data(), t_data, sizeof(T));

            std::ranges::reverse(data);

            std::memcpy(&object, data.data(), sizeof(T));
        }
        else 
        {
            std::memcpy(&object, t_data, sizeof(T));
        }

        return object;
    }
}

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
    Collection::Collection(const Collection& collection)
    {
        this->operator=(collection);
    }

    Collection::Collection(Collection&& collection)
    {
        this->operator=(std::move(collection));
    }

    Collection& Collection::operator=(const Collection& collection)
    {
        data = collection.data;

        return *this;
    }

    Collection& Collection::operator=(Collection&& collection)
    {
        data = std::move(collection.data);

        return *this;
    }

    Collection& Collection::clearOnSending(const bool value)
    {
        clear_on_sending = value;

        return *this;
    }

    Collection& Collection::clearOnReceiving(const bool value)
    {
        clear_on_receiving = value;

        return *this;
    }

    Collection& Collection::dropOnRetrieving(const bool value)
    {
        drop_on_retrieving = value;

        return *this;
    }

    Collection& Collection::clear()
    {
        data.clear();

        return *this;
    }

    size_t Collection::getSize() const 
    {
        return data.size();
    }

    void Collection::cropBytes(const size_t bytes_count)
    {
        data.erase(data.begin(), data.begin() + bytes_count);
    }

    std::byte* Collection::getData()
    {
        return data.data();
    }

    const std::byte* Collection::getData() const
    {
        return data.data();
    }

    void Collection::resize(const size_t new_size)
    {
        data.resize(new_size);
    }
    
    bool Collection::isEmpty() const
    {
        return !getSize();
    }

    bool Collection::clearsOnSending() const
    {
        return clear_on_sending;
    }

    bool Collection::clearsOnReceiving() const
    {
        return clear_on_receiving;
    }

    bool Collection::dropsOnRetrieving() const
    {
        return drop_on_retrieving;
    }        

    size_t Collection::getDataSize() const 
    {
        return data.size();
    }

    template <typename T>
    std::vector<std::byte> Collection::prepareDataForInserting(const T& source)
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
    T Collection::prepareDataForExtracting(const std::byte* const data)
    {
        T object;

        if(std::endian::native != std::endian::big)
        {
            std::vector<std::byte> data {sizeof(T)};

            std::memcpy(data, &object, sizeof(T));

            std::ranges::reverse(data);

            std::memcpy(&object, data.data(), sizeof(T));
        }
        else 
        {
            std::memcpy(&object, data, sizeof(T));
        }

        return object;
    }
}
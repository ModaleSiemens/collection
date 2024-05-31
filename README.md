# collection
Fancy wrapper around a dynamic *byte* array. Can be overloaded to work with files, sockets, etc.
## Overloading Sending and Receiving
### Sending
```cpp
template <>
inline A& mdsm::operator<<<A>(A& a, const mdsm::Collection& collection) 
{
    //...

    return a;
}
```
### Receiving
```cpp
template <>
inline A& mdsm::operator>><A>(A& a, mdsm::Collection& collection)
{
    //...

    return a;
}
```

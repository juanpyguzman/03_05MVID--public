#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <functional>

template <class T>
class Vector {
  public:
    Vector() {}                                             //Constructor
    ~Vector() {}                                            //Destructor

    T& push_back(const T& v) {}	                            //Add element at the end
    T& push_back(T&& v) {}

    T& emplace_back(T&& v) {}                               //Construct and insert element

    void clear() {}                	                        //Clean the vector
    void resize(size_t n) {}      	                        //Resize the vector
    void reserve(size_t n) {}  	                            //Reserve elements

    void erase(std::size_t first, std::size_t last) {}	    //Erase elements in a range
    T pop_back() {}    	                                    //Erase the last element

    size_t size() {}     	                                //Size of vector
    size_t capacity() {}	                                //Capacity of the vector
    bool empty() {}    	                                    //Verifies if the vector is empty

    T& operator[](size_t idx) {}        	                //Access to an element
    const T& operator[](size_t idx) const {}

    T& back() {}          	                                //Access to the last element
    const T& back() const {}

    void foreach(std::function<void(const T&)> f) const {}  //Loops through the vector and executes the function for each element
    void foreach(std::function<void(T&)> f) {}

    void swapElement(size_t pos, T* other) {}          	    //Swaps an element of the vector
    void swap(Vector& other) {}                             //Swaps the whole vector
  private:
};

#endif
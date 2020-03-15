#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <functional>
#include <mutex>
#include <vector>
#include <thread>
#include <algorithm>

template <class T>
class Vector : public std::vector<T> {

  public:
    //Constructor
    Vector() {}                                             
    
    //Destructor
    ~Vector() {}                                        

	//Add element at the end
    void push_back(const T& v) {
		std::lock_guard<std::mutex> guard(_mutex);
		std::vector<T>::push_back(v);
	}	                          

    void push_back(T&& v) {
		std::lock_guard<std::mutex> guard(_mutex);
		vector<T>::push_back(v);
	}

    //Construct and insert element
    T& emplace_back(T&& v) {
        std::lock_guard<std::mutex> guard(_mutex);
        return std::vector<T>::emplace_back(v);
    }                               

    //Clean the vector
    void clear() {
        std::lock_guard<std::mutex> guard(_mutex);
        std::vector<T>::clear();
    }

    //Resize the vector
    void resize(size_t n) {
        std::lock_guard<std::mutex> guard(_mutex);
        std::vector<T>::resize(n);
    }      	   

    //Reserve elements
    void reserve(size_t n) {
        std::lock_guard<std::mutex> guard(_mutex);
        std::vector<T>::reserve(n);
    }  	                            

    //Erase elements in a range
    void erase(std::size_t first, std::size_t last) {
        std::lock_guard<std::mutex> guard(_mutex);
        std::vector<T>::erase(first, last);
    }	    
    
    //Erase the last element
    void pop_back() {
        std::lock_guard<std::mutex> guard(_mutex);
        std::vector<T>::pop_back();
    }    	                                    

    //Size of vector      
    size_t size() {
        std::lock_guard<std::mutex> guard(_mutex);
        return std::vector<T>::size();
    }

    //Capacity of the vector
    size_t capacity() {
        std::lock_guard<std::mutex> guard(_mutex);
        return std::vector<T>::capacity();
    }
    
    //Verifies if the vector is empty
    bool empty() {
        std::lock_guard<std::mutex> guard(_mutex);
        return std::vector<T>::empty();
    }    	                                    

    //Access to an element
    T& operator[](size_t idx) {
        std::lock_guard<std::mutex> guard(_mutex);
        return std::vector<T>::operator[](idx);
    }        	                
    
    
    const T& operator[](size_t idx) const {
        std::lock_guard<std::mutex> guard(_mutex);
        return std::vector<T>::operator[](idx);
    }

    //Access to the last element
    T& back() {
        std::lock_guard<std::mutex> guard(_mutex);
        return std::vector<T>::back();
    }          	                                
    const T& back() const {
        std::lock_guard<std::mutex> guard(_mutex);
        return std::vector<T>::back();
    }

    //Loops through the vector and executes the function for each element
    void foreach(std::function<void(const T&)> f) const {
        std::lock_guard<std::mutex> guard(_mutex);
        std::for_each(std::vector<T>::begin(), std::vector<T>::end(), f);
    }

    void foreach(std::function<void(T&)> f) {
        std::lock_guard<std::mutex> guard(_mutex);
        std::for_each(std::vector<T>::begin(), std::vector<T>::end(), f);
    }

    //Swaps an element of the vector
    void swapElement(size_t pos, const T& element) {
        std::lock_guard<std::mutex> guard(_mutex);
        std::vector<T>::operator[](pos) = element;
    }

    //Swaps the whole vector
    void swap(Vector& other) {
        std::lock_guard<std::mutex> guard(_mutex);
        std::vector<T>::swap(other);
    }                             
  
    private:
	  std::mutex _mutex;
};

#endif
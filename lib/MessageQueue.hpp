#pragma once
#include <memory>
#include <queue>
#include <mutex> 
#include <condition_variable>

template<typename T> 
class MessageQueue 
{ 
public: 
    MessageQueue() {} 
    MessageQueue(const MessageQueue&) = delete; 
    void send(T & message) 
    { 
        std::lock_guard<std::mutex> lg(mutex_m); 
        queue_m.push(std::move(message)); 
        dataCondition_m.notify_one(); 
    } 
    void receive(T&t) 
    { 
        std::unique_lock<std::mutex> ul(mutex_m); 
        dataCondition_m.wait(ul, [this] {return !queue_m.empty(); }); 
        t = queue_m.front(); 
        queue_m.pop(); 
    } 
    std::shared_ptr<T> receive() 
    { 
        std::unique_lock<std::mutex> ul(mutex_m); 
        dataCondition_m.wait(ul, [this] {return !queue_m.empty(); }); 
 
        std::shared_ptr<T> res(std::make_shared<T>(queue_m.front())); 
        queue_m.pop(); 
        return res; 
    } 
    bool TryReceive(T &t) 
    { 
        std::lock_guard<std::mutex> lg(mutex_m); 
        if (queue_m.empty()) 
            return false; 
 
        t = queue_m.front(); 
        queue_m.pop(); 
        return true; 
    }

    std::shared_ptr<T> TryReceive() 
    { 
        std::lock_guard<std::mutex> lg(mutex_m); 
        if (queue_m.empty()) 
            return std::shared_ptr<T>(); 
            
        std::shared_ptr<T> res(std::make_shared<T>(queue_m.front())); 
        queue_m.pop(); 
        return res; 
    } 
 
    bool isEmpty() 
    { 
        std::lock_guard<std::mutex> lg(mutex_m); 
        return queue_m.empty(); 
    }
    
    size_t size() const
    {
        std::lock_guard<std::mutex> lg(mutex_m); 
        return queue_m.size();
    }
 private: 
    mutable std::mutex mutex_m; 
    std::queue<T> queue_m; 
    std::condition_variable dataCondition_m; 

}; 


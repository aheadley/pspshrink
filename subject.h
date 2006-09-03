#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include <assert.h>
#include <pthread.h>

#include "observer.h"

#include <iostream>
using namespace std;

template <class NotMsgSub>
class Subject
{
public:
    Subject()
    {
        int ret = pthread_mutex_init(&m_ObserversMutex, NULL);
        assert(ret == 0);
    }
    
    ~Subject()
    {
        int ret = pthread_mutex_destroy(&m_ObserversMutex);
        assert(ret == 0);
    }
      
    void subscribe (Observer<NotMsgSub>& observer)
    {
        pthread_mutex_lock(&m_ObserversMutex);
        removeObserver(observer);
        m_Observers.push_back(&observer);
        pthread_mutex_unlock(&m_ObserversMutex);
    }
      
    void unsubscribe(Observer<NotMsgSub>& observer)
    {
        pthread_mutex_lock(&m_ObserversMutex);
        removeObserver(observer);
        pthread_mutex_unlock(&m_ObserversMutex);
    }
    
protected:

    void notify (const NotMsgSub& aMsg)
    {
        std::vector< Observer<NotMsgSub>* > observers;
        {
            pthread_mutex_lock(&m_ObserversMutex);
            observers = m_Observers;
            pthread_mutex_unlock(&m_ObserversMutex);
        }
        for(typename std::vector< Observer<NotMsgSub>* >::iterator myIt = observers.begin();
            myIt != observers.end();
            myIt++)
        {
            (*myIt)->update(aMsg);
        }
    }
    
    void removeObserver(Observer<NotMsgSub>& observer)
    {
        for (typename std::vector< Observer<NotMsgSub>* >::iterator myIt = m_Observers.begin();
                myIt != m_Observers.end();
                myIt++)
        {
            if ((*myIt) == &observer)
            {
                m_Observers.erase(myIt);
                return;
            }
        }
    }

private:
	Subject(const Subject& );
    const Subject& operator=(const Subject& );

    std::vector<Observer<NotMsgSub>* > 	m_Observers;
    pthread_mutex_t						m_ObserversMutex;
};

#endif

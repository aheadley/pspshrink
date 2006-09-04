#ifndef OBSERVER_H
#define OBSERVER_H

template <class Notification>
class Observer
{
public:
    virtual ~Observer() {}

    virtual void update (const Notification& notification)=0;
protected:
    Observer() {}
    
    //non copyable
    Observer(const Observer& );
    const Observer& operator=(const Observer& );
};

#endif

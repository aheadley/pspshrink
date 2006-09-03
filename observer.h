#ifndef OBSERVER_H
#define OBSERVER_H

template <class NotMsg>
class Observer
{
public:
    virtual ~Observer(){}

    virtual void update (const NotMsg& aMsg)=0;
protected:
    Observer (){}
    
    //non copyable
    Observer(const Observer& );
    const Observer& operator=(const Observer& );
};
#endif

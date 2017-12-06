#ifndef _EVENTS_H_
#define _EVENTS_H_

class EventArgs {
  public:
    void* m_Sender;
};

class Event
{
  public:
    virtual void invoke(EventArgs* args) = 0;
};


template<class T, class ARG> class EventHandler : public Event
{
  public:
    EventHandler(T* obj, void(T::* func)(ARG*)) {
      m_Obj  = obj;
      m_Func = func;
    }

    virtual void invoke(EventArgs* args) {
      (m_Obj->*m_Func)((ARG*)args);
    }

  private:
    T* m_Obj;
    void (T::* m_Func)(ARG*);
};

#endif


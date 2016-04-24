#ifndef SIGNAL_handleH
#define SIGNAL_handleH

/**<
    目前该信号槽机制最大限度只支持9个参数的传递
    若要支持更多参数传递则将代码加在本文件末尾
 */

#include <vector>
#define c_emit
#define c_slots
#define c_signals public
#define c_connect(sender, signal, receiver, slot) ((sender) ->signal.Bind(receiver, slot))

///////////////////////////////////////////no param signal-slot////////////////////////////////////////////////////
/**<
    槽接口类
 */
class SlotBase
{
public:
    virtual ~SlotBase() {}
    virtual void exec() = 0;
};

/**<
    槽模板类
 */
template<class T>
class SlotImpl : public SlotBase
{
public:
    SlotImpl(T *pObj, void (T::*func)())
    {
        m_pObj = pObj;
        m_Func = func;
    }

    void exec()
    {
        (m_pObj ->*m_Func)();
    }

private:
    T *m_pObj;
    void (T::*m_Func)();
};

/**<
    槽
 */
class Slot
{
public:
    template<class T>
    Slot(T *pObj, void (T::*func)())
    {
        m_pSlotBase = new SlotImpl<T>(pObj, func);
    }

    ~Slot()
    {
        if (m_pSlotBase != NULL) { delete m_pSlotBase; m_pSlotBase = NULL; }
    }

    void exec()
    {
        m_pSlotBase ->exec();
    }

private:
    SlotBase *m_pSlotBase;
};

/**<
    信号
 */
class Signal
{
public:
    template<class T>
    void Bind(T *pObj, void (T::*func)())
    {
        m_pSlotVec.push_back(new Slot(pObj, func));
    }

    ~Signal()
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            if (m_pSlotVec[i] != NULL) { delete m_pSlotVec[i]; m_pSlotVec[i] = NULL; }
        }
    }

    void operator()()
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            m_pSlotVec[i] ->exec();
        }
    }

private:
    std::vector< Slot* > m_pSlotVec;
};

///////////////////////////////////////////one param signal-slot////////////////////////////////////////////////////
/**<
    槽接口类
 */
template<class T1>
class SlotBase1
{
public:
    virtual ~SlotBase1() {}
    virtual void exec(T1 t1) = 0;
};

/**<
    槽模板类
 */
template<class T, class T1>
class SlotImpl1 : public SlotBase1<T1>
{
public:
    SlotImpl1(T *pObj, void (T::*func)(T1))
    {
        m_pObj = pObj;
        m_Func = func;
    }

    void exec(T1 t1)
    {
        (m_pObj ->*m_Func)(t1);
    }

private:
    T *m_pObj;
    void (T::*m_Func)(T1);
};

/**<
    槽
 */
template<class T1>
class Slot1
{
public:
    template<class T>
    Slot1(T *pObj, void (T::*func)(T1))
    {
        m_pSlotBase = new SlotImpl1<T, T1>(pObj, func);
    }

    ~Slot1()
    {
        if (m_pSlotBase != NULL) { delete m_pSlotBase; m_pSlotBase = NULL; }
    }

    void exec(T1 t1)
    {
        m_pSlotBase ->exec(t1);
    }

private:
    SlotBase1<T1> *m_pSlotBase;
};

/**<
    信号
 */
template<class T1>
class Signal1
{
public:
    template<class T>
    void Bind(T *pObj, void (T::*func)(T1))
    {
        m_pSlotVec.push_back(new Slot1<T1>(pObj, func));
    }

    ~Signal1()
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            if (m_pSlotVec[i] != NULL) { delete m_pSlotVec[i]; m_pSlotVec[i] = NULL; }
        }
    }

    void operator()(T1 t1)
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            m_pSlotVec[i] ->exec(t1);
        }
    }

private:
    std::vector< Slot1<T1>* > m_pSlotVec;
};

///////////////////////////////////////////two param signal-slot////////////////////////////////////////////////////
/**<
    槽接口类
 */
template<class T1, class T2>
class SlotBase2
{
public:
    virtual ~SlotBase2() {}
    virtual void exec(T1 t1, T2 t2) = 0;
};

/**<
    槽模板类
 */
template<class T, class T1, class T2>
class SlotImpl2 : public SlotBase2<T1, T2>
{
public:
    SlotImpl2(T *pObj, void (T::*func)(T1, T2))
    {
        m_pObj = pObj;
        m_Func = func;
    }

    void exec(T1 t1, T2 t2)
    {
        (m_pObj ->*m_Func)(t1, t2);
    }

private:
    T *m_pObj;
    void (T::*m_Func)(T1, T2);
};

/**<
    槽
 */
template<class T1, class T2>
class Slot2
{
public:
    template<class T>
    Slot2(T *pObj, void (T::*func)(T1, T2))
    {
        m_pSlotBase = new SlotImpl2<T, T1, T2>(pObj, func);
    }

    ~Slot2()
    {
        if (m_pSlotBase != NULL) { delete m_pSlotBase; m_pSlotBase = NULL; }
    }

    void exec(T1 t1, T2 t2)
    {
        m_pSlotBase ->exec(t1, t2);
    }

private:
    SlotBase2<T1, T2> *m_pSlotBase;
};

/**<
    信号
 */
template<class T1, class T2>
class Signal2
{
public:
    template<class T>
    void Bind(T *pObj, void (T::*func)(T1, T2))
    {
        m_pSlotVec.push_back(new Slot2<T1, T2>(pObj, func));
    }

    ~Signal2()
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            if (m_pSlotVec[i] != NULL) { delete m_pSlotVec[i]; m_pSlotVec[i] = NULL; }
        }
    }

    void operator()(T1 t1, T2 t2)
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            m_pSlotVec[i] ->exec(t1, t2);
        }
    }

private:
    std::vector< Slot2<T1, T2>* > m_pSlotVec;
};

///////////////////////////////////////////three param signal-slot////////////////////////////////////////////////////
/**<
    槽接口类
 */
template<class T1, class T2, class T3>
class SlotBase3
{
public:
    virtual ~SlotBase3() {}
    virtual void exec(T1 t1, T2 t2, T3 t3) = 0;
};

/**<
    槽模板类
 */
template<class T, class T1, class T2, class T3>
class SlotImpl3 : public SlotBase3<T1, T2, T3>
{
public:
    SlotImpl3(T *pObj, void (T::*func)(T1, T2, T3))
    {
        m_pObj = pObj;
        m_Func = func;
    }

    void exec(T1 t1, T2 t2, T3 t3)
    {
        (m_pObj ->*m_Func)(t1, t2, t3);
    }

private:
    T *m_pObj;
    void (T::*m_Func)(T1, T2, T3);
};

/**<
    槽
 */
template<class T1, class T2, class T3>
class Slot3
{
public:
    template<class T>
    Slot3(T *pObj, void (T::*func)(T1, T2, T3))
    {
        m_pSlotBase = new SlotImpl3<T, T1, T2, T3>(pObj, func);
    }

    ~Slot3()
    {
        if (m_pSlotBase != NULL) { delete m_pSlotBase; m_pSlotBase = NULL; }
    }

    void exec(T1 t1, T2 t2, T3 t3)
    {
        m_pSlotBase ->exec(t1, t2, t3);
    }

private:
    SlotBase3<T1, T2, T3> *m_pSlotBase;
};

/**<
    信号
 */
template<class T1, class T2, class T3>
class Signal3
{
public:
    template<class T>
    void Bind(T *pObj, void (T::*func)(T1, T2, T3))
    {
        m_pSlotVec.push_back(new Slot3<T1, T2, T3>(pObj, func));
    }

    ~Signal3()
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            if (m_pSlotVec[i] != NULL) { delete m_pSlotVec[i]; m_pSlotVec[i] = NULL; }
        }
    }

    void operator()(T1 t1, T2 t2, T3 t3)
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            m_pSlotVec[i] ->exec(t1, t2, t3);
        }
    }

private:
    std::vector< Slot3<T1, T2, T3>* > m_pSlotVec;
};

///////////////////////////////////////////four param signal-slot////////////////////////////////////////////////////
/**<
    槽接口类
 */
template<class T1, class T2, class T3, class T4>
class SlotBase4
{
public:
    virtual ~SlotBase4() {}
    virtual void exec(T1 t1, T2 t2, T3 t3, T4 t4) = 0;
};

/**<
    槽模板类
 */
template<class T, class T1, class T2, class T3, class T4>
class SlotImpl4 : public SlotBase4<T1, T2, T3, T4>
{
public:
    SlotImpl4(T *pObj, void (T::*func)(T1, T2, T3, T4))
    {
        m_pObj = pObj;
        m_Func = func;
    }

    void exec(T1 t1, T2 t2, T3 t3, T4 t4)
    {
        (m_pObj ->*m_Func)(t1, t2, t3, t4);
    }

private:
    T *m_pObj;
    void (T::*m_Func)(T1, T2, T3, T4);
};

/**<
    槽
 */
template<class T1, class T2, class T3, class T4>
class Slot4
{
public:
    template<class T>
    Slot4(T *pObj, void (T::*func)(T1, T2, T3, T4))
    {
        m_pSlotBase = new SlotImpl4<T, T1, T2, T3, T4>(pObj, func);
    }

    ~Slot4()
    {
        if (m_pSlotBase != NULL) { delete m_pSlotBase; m_pSlotBase = NULL; }
    }

    void exec(T1 t1, T2 t2, T3 t3, T4 t4)
    {
        m_pSlotBase ->exec(t1, t2, t3, t4);
    }

private:
    SlotBase4<T1, T2, T3, T4> *m_pSlotBase;
};

/**<
    信号
 */
template<class T1, class T2, class T3, class T4>
class Signal4
{
public:
    template<class T>
    void Bind(T *pObj, void (T::*func)(T1, T2, T3, T4))
    {
        m_pSlotVec.push_back(new Slot4<T1, T2, T3, T4>(pObj, func));
    }

    ~Signal4()
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            if (m_pSlotVec[i] != NULL) { delete m_pSlotVec[i]; m_pSlotVec[i] = NULL; }
        }
    }

    void operator()(T1 t1, T2 t2, T3 t3, T4 t4)
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            m_pSlotVec[i] ->exec(t1, t2, t3, t4);
        }
    }

private:
    std::vector< Slot4<T1, T2, T3, T4>* > m_pSlotVec;
};

///////////////////////////////////////////five param signal-slot////////////////////////////////////////////////////
/**<
    槽接口类
 */
template<class T1, class T2, class T3, class T4, class T5>
class SlotBase5
{
public:
    virtual ~SlotBase5() {}
    virtual void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) = 0;
};

/**<
    槽模板类
 */
template<class T, class T1, class T2, class T3, class T4, class T5>
class SlotImpl5 : public SlotBase5<T1, T2, T3, T4, T5>
{
public:
    SlotImpl5(T *pObj, void (T::*func)(T1, T2, T3, T4, T5))
    {
        m_pObj = pObj;
        m_Func = func;
    }

    void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
    {
        (m_pObj ->*m_Func)(t1, t2, t3, t4, t5);
    }

private:
    T *m_pObj;
    void (T::*m_Func)(T1, T2, T3, T4, T5);
};

/**<
    槽
 */
template<class T1, class T2, class T3, class T4, class T5>
class Slot5
{
public:
    template<class T>
    Slot5(T *pObj, void (T::*func)(T1, T2, T3, T4, T5))
    {
        m_pSlotBase = new SlotImpl5<T, T1, T2, T3, T4, T5>(pObj, func);
    }

    ~Slot5()
    {
        if (m_pSlotBase != NULL) { delete m_pSlotBase; m_pSlotBase = NULL; }
    }

    void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
    {
        m_pSlotBase ->exec(t1, t2, t3, t4, t5);
    }

private:
    SlotBase5<T1, T2, T3, T4, T5> *m_pSlotBase;
};

/**<
    信号
 */
template<class T1, class T2, class T3, class T4, class T5>
class Signal5
{
public:
    template<class T>
    void Bind(T *pObj, void (T::*func)(T1, T2, T3, T4, T5))
    {
        m_pSlotVec.push_back(new Slot5<T1, T2, T3, T4, T5>(pObj, func));
    }

    ~Signal5()
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            if (m_pSlotVec[i] != NULL) { delete m_pSlotVec[i]; m_pSlotVec[i] = NULL; }
        }
    }

    void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            m_pSlotVec[i] ->exec(t1, t2, t3, t4, t5);
        }
    }

private:
    std::vector< Slot5<T1, T2, T3, T4, T5>* > m_pSlotVec;
};

///////////////////////////////////////////six param signal-slot////////////////////////////////////////////////////
/**<
    槽接口类
 */
template<class T1, class T2, class T3, class T4, class T5, class T6>
class SlotBase6
{
public:
    virtual ~SlotBase6() {}
    virtual void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) = 0;
};

/**<
    槽模板类
 */
template<class T, class T1, class T2, class T3, class T4, class T5, class T6>
class SlotImpl6 : public SlotBase6<T1, T2, T3, T4, T5, T6>
{
public:
    SlotImpl6(T *pObj, void (T::*func)(T1, T2, T3, T4, T5, T6))
    {
        m_pObj = pObj;
        m_Func = func;
    }

    void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
    {
        (m_pObj ->*m_Func)(t1, t2, t3, t4, t5, t6);
    }

private:
    T *m_pObj;
    void (T::*m_Func)(T1, T2, T3, T4, T5, T6);
};

/**<
    槽
 */
template<class T1, class T2, class T3, class T4, class T5, class T6>
class Slot6
{
public:
    template<class T>
    Slot6(T *pObj, void (T::*func)(T1, T2, T3, T4, T5, T6))
    {
        m_pSlotBase = new SlotImpl6<T, T1, T2, T3, T4, T5, T6>(pObj, func);
    }

    ~Slot6()
    {
        if (m_pSlotBase != NULL) { delete m_pSlotBase; m_pSlotBase = NULL; }
    }

    void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
    {
        m_pSlotBase ->exec(t1, t2, t3, t4, t5, t6);
    }

private:
    SlotBase6<T1, T2, T3, T4, T5, T6> *m_pSlotBase;
};

/**<
    信号
 */
template<class T1, class T2, class T3, class T4, class T5, class T6>
class Signal6
{
public:
    template<class T>
    void Bind(T *pObj, void (T::*func)(T1, T2, T3, T4, T5, T6))
    {
        m_pSlotVec.push_back(new Slot6<T1, T2, T3, T4, T5, T6>(pObj, func));
    }

    ~Signal6()
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            if (m_pSlotVec[i] != NULL) { delete m_pSlotVec[i]; m_pSlotVec[i] = NULL; }
        }
    }

    void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            m_pSlotVec[i] ->exec(t1, t2, t3, t4, t5, t6);
        }
    }

private:
    std::vector< Slot6<T1, T2, T3, T4, T5, T6>* > m_pSlotVec;
};

///////////////////////////////////////////seven param signal-slot////////////////////////////////////////////////////
/**<
    槽接口类
 */
template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
class SlotBase7
{
public:
    virtual ~SlotBase7() {}
    virtual void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7) = 0;
};

/**<
    槽模板类
 */
template<class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
class SlotImpl7 : public SlotBase7<T1, T2, T3, T4, T5, T6, T7>
{
public:
    SlotImpl7(T *pObj, void (T::*func)(T1, T2, T3, T4, T5, T6, T7))
    {
        m_pObj = pObj;
        m_Func = func;
    }

    void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
    {
        (m_pObj ->*m_Func)(t1, t2, t3, t4, t5, t6, t7);
    }

private:
    T *m_pObj;
    void (T::*m_Func)(T1, T2, T3, T4, T5, T6, T7);
};

/**<
    槽
 */
template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
class Slot7
{
public:
    template<class T>
    Slot7(T *pObj, void (T::*func)(T1, T2, T3, T4, T5, T6, T7))
    {
        m_pSlotBase = new SlotImpl7<T, T1, T2, T3, T4, T5, T6, T7>(pObj, func);
    }

    ~Slot7()
    {
        if (m_pSlotBase != NULL) { delete m_pSlotBase; m_pSlotBase = NULL; }
    }

    void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
    {
        m_pSlotBase ->exec(t1, t2, t3, t4, t5, t6, t7);
    }

private:
    SlotBase7<T1, T2, T3, T4, T5, T6, T7> *m_pSlotBase;
};

/**<
    信号
 */
template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
class Signal7
{
public:
    template<class T>
    void Bind(T *pObj, void (T::*func)(T1, T2, T3, T4, T5, T6, T7))
    {
        m_pSlotVec.push_back(new Slot7<T1, T2, T3, T4, T5, T6, T7>(pObj, func));
    }

    ~Signal7()
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            if (m_pSlotVec[i] != NULL) { delete m_pSlotVec[i]; m_pSlotVec[i] = NULL; }
        }
    }

    void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            m_pSlotVec[i] ->exec(t1, t2, t3, t4, t5, t6, t7);
        }
    }

private:
    std::vector< Slot7<T1, T2, T3, T4, T5, T6, T7>* > m_pSlotVec;
};

///////////////////////////////////////////eight param signal-slot////////////////////////////////////////////////////
/**<
    槽接口类
 */
template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
class SlotBase8
{
public:
    virtual ~SlotBase8() {}
    virtual void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8) = 0;
};

/**<
    槽模板类
 */
template<class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
class SlotImpl8 : public SlotBase8<T1, T2, T3, T4, T5, T6, T7, T8>
{
public:
    SlotImpl8(T *pObj, void (T::*func)(T1, T2, T3, T4, T5, T6, T7, T8))
    {
        m_pObj = pObj;
        m_Func = func;
    }

    void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
    {
        (m_pObj ->*m_Func)(t1, t2, t3, t4, t5, t6, t7, t8);
    }

private:
    T *m_pObj;
    void (T::*m_Func)(T1, T2, T3, T4, T5, T6, T7, T8);
};

/**<
    槽
 */
template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
class Slot8
{
public:
    template<class T>
    Slot8(T *pObj, void (T::*func)(T1, T2, T3, T4, T5, T6, T7, T8))
    {
        m_pSlotBase = new SlotImpl8<T, T1, T2, T3, T4, T5, T6, T7, T8>(pObj, func);
    }

    ~Slot8()
    {
        if (m_pSlotBase != NULL) { delete m_pSlotBase; m_pSlotBase = NULL; }
    }

    void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
    {
        m_pSlotBase ->exec(t1, t2, t3, t4, t5, t6, t7, t8);
    }

private:
    SlotBase8<T1, T2, T3, T4, T5, T6, T7, T8> *m_pSlotBase;
};

/**<
    信号
 */
template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
class Signal8
{
public:
    template<class T>
    void Bind(T *pObj, void (T::*func)(T1, T2, T3, T4, T5, T6, T7, T8))
    {
        m_pSlotVec.push_back(new Slot8<T1, T2, T3, T4, T5, T6, T7, T8>(pObj, func));
    }

    ~Signal8()
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            if (m_pSlotVec[i] != NULL) { delete m_pSlotVec[i]; m_pSlotVec[i] = NULL; }
        }
    }

    void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            m_pSlotVec[i] ->exec(t1, t2, t3, t4, t5, t6, t7, t8);
        }
    }

private:
    std::vector< Slot8<T1, T2, T3, T4, T5, T6, T7, T8>* > m_pSlotVec;
};

///////////////////////////////////////////nine param signal-slot////////////////////////////////////////////////////
/**<
    槽接口类
 */
template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
class SlotBase9
{
public:
    virtual ~SlotBase9() {}
    virtual void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9) = 0;
};

/**<
    槽模板类
 */
template<class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
class SlotImpl9 : public SlotBase9<T1, T2, T3, T4, T5, T6, T7, T8, T9>
{
public:
    SlotImpl9(T *pObj, void (T::*func)(T1, T2, T3, T4, T5, T6, T7, T8, T9))
    {
        m_pObj = pObj;
        m_Func = func;
    }

    void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
    {
        (m_pObj ->*m_Func)(t1, t2, t3, t4, t5, t6, t7, t8, t9);
    }

private:
    T *m_pObj;
    void (T::*m_Func)(T1, T2, T3, T4, T5, T6, T7, T8, T9);
};

/**<
    槽
 */
template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
class Slot9
{
public:
    template<class T>
    Slot9(T *pObj, void (T::*func)(T1, T2, T3, T4, T5, T6, T7, T8, T9))
    {
        m_pSlotBase = new SlotImpl9<T, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pObj, func);
    }

    ~Slot9()
    {
        if (m_pSlotBase != NULL) { delete m_pSlotBase; m_pSlotBase = NULL; }
    }

    void exec(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
    {
        m_pSlotBase ->exec(t1, t2, t3, t4, t5, t6, t7, t8, t9);
    }

private:
    SlotBase9<T1, T2, T3, T4, T5, T6, T7, T8, T9> *m_pSlotBase;
};

/**<
    信号
 */
template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
class Signal9
{
public:
    template<class T>
    void Bind(T *pObj, void (T::*func)(T1, T2, T3, T4, T5, T6, T7, T8, T9))
    {
        m_pSlotVec.push_back(new Slot9<T1, T2, T3, T4, T5, T6, T7, T8, T9>(pObj, func));
    }

    ~Signal9()
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            if (m_pSlotVec[i] != NULL) { delete m_pSlotVec[i]; m_pSlotVec[i] = NULL; }
        }
    }

    void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
    {
        for (int i = 0; i < (int)m_pSlotVec.size(); ++i)
        {
            m_pSlotVec[i] ->exec(t1, t2, t3, t4, t5, t6, t7, t8, t9);
        }
    }

private:
    std::vector< Slot9<T1, T2, T3, T4, T5, T6, T7, T8, T9>* > m_pSlotVec;
};

#endif // SIGNAL_handleH

// =================================================================== //
// Time-stamp: <23 Jun 03 10:42:45 Jean-Guillaume.Dumas@imag.fr>
// =================================================================== //
#ifndef __Agora_Container__
#define __Agora_Container__

// #include <vector>
// typedef std::vector AgoraContainer;

#include <iostream>

template<int POSS_SIZE, typename Elem> struct AgoraContainer {
private:
    typedef AgoraContainer<POSS_SIZE, Elem> Self_t;
    Elem _container[ POSS_SIZE ];
    Elem * _finish;
public:
    typedef Elem* iterator;
    typedef const Elem* const_iterator;
    AgoraContainer() : _finish(_container) { }
    AgoraContainer(int s) : _finish(_container+s) { }
    Case_t size() const { return Case_t(_finish - _container); }
    void clear() { _finish = _container; }
    void resize(int s) { _finish = _container + s; }
    iterator begin() { return _container; }
    iterator end() { return _finish; }
    const_iterator begin() const { return const_iterator(_container); }
    const_iterator end() const { return const_iterator(_finish); }
    void push_back(const Elem& c) {
        *_finish = c; ++_finish;
    }
};

#endif

/*
================================================================================
🔹 Usage Guide for Action<T...> 🔹
================================================================================

✅ To subscribe/unsubscribe a member function:

    action.subscribe(&ClassName::MethodName, &instance);
    action.unsubscribe(&ClassName::MethodName, &instance);

No macros needed. No external binding required.

--------------------------------------------------------------------------------
❗ IMPORTANT for Overloaded Methods:
--------------------------------------------------------------------------------
If the method you are subscribing is **overloaded** (multiple functions
with same name but different parameters), you **must disambiguate** it
using a static_cast.

Example:

    class MyClass {
    public:
        void Foo();       // Overload 1
        void Foo(int x);  // Overload 2
    };

You must specify exactly **which Foo** you mean like this:

    // Subscribing the void Foo() version
    action.subscribe(
        static_cast<void (MyClass::*)()>(&MyClass::Foo),
        &instance);

    // Subscribing the void Foo(int) version
    action.subscribe(
        static_cast<void (MyClass::*)(int)>(&MyClass::Foo),
        &instance);

Same applies for `unsubscribe`.

--------------------------------------------------------------------------------
🧠 Why is this needed?
--------------------------------------------------------------------------------
- C++ cannot automatically choose between overloaded functions based
  only on name.
- It requires an explicit cast to know exactly which overload you intend
  to subscribe.
- This is **normal C++ behavior** and not a limitation of the Action system.

--------------------------------------------------------------------------------
✅ After static_cast, the Action system will work safely and correctly.
================================================================================
*/


#pragma once
#include <functional>
#include <unordered_map>
#include <iostream>
#include <typeindex>
#include <tuple>
#include <string_view>


#if defined(_MSC_VER)
#define METHOD_SIGNATURE __FUNCSIG__
#else
#define METHOD_SIGNATURE __PRETTY_FUNCTION__
#endif

namespace AnalyticalApproach::EventSystem
{
    template <typename... Args>
    class Action
    {
    public:
        using Callback = std::function<void(Args...)>;

        struct Key
        {
            std::type_index type;
            void *objPtr;
            size_t methodId;

            bool operator==(const Key &other) const
            {
                return type == other.type && objPtr == other.objPtr && methodId == other.methodId;
            }
        };

        struct KeyHash
        {
            std::size_t operator()(const Key &key) const
            {
                return std::hash<void *>()(key.objPtr) ^ std::hash<size_t>()(key.methodId) ^ key.type.hash_code();
            }
        };

    private:
        // Helper: Compute MethodID from Method Pointer Type
        template<typename T, typename MethodPtr>
        static constexpr size_t computeMethodId(MethodPtr)
        {
            return std::hash<std::string_view>{}(METHOD_SIGNATURE);
        }

    public:

        /*
        Usage: 
            action.subscribe(&ClassName::MethodName, &Instance);
        */
        template <typename T>
        void subscribe(void (T::*method)(Args...), T *instance)
        {
            size_t methodId = computeMethodId<T>(method);

            Key key{std::type_index(typeid(T)), static_cast<void *>(instance), methodId};
            callbacks[key] = [method, instance](Args... args)
            {
                (instance->*method)(args...);
            };
        }

        /*
        Usage: 
            action.unsubscribe(&ClassName::MethodName, &Instance);
        */
        template <typename T>
        void unsubscribe(void (T::*method)(Args...), T *instance)
        {
            size_t methodId = computeMethodId<T>(method);

            Key key{std::type_index(typeid(T)), static_cast<void *>(instance), methodId};
            callbacks.erase(key);
        }

        void operator()(Args... args) const
        {
            for (const auto &[key, cb] : callbacks)
            {
                cb(args...);
            }
        }

    private:
        std::unordered_map<Key, Callback, KeyHash> callbacks;
    };
}

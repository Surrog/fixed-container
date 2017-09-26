#ifndef FIXED_TEST_STRUCT_HPP
#define FIXED_TEST_STRUCT_HPP

#include <algorithm>
#include <cstring>

struct test
{
    test(const char* val)
        : _val(val)
    {
    }

    const char* _val = nullptr;
    bool operator==(const test& rval)
    {
        return std::equal(_val, _val + strlen(_val), rval._val,
            rval._val + strlen(rval._val));
    }
};

struct test_construct
{
    test_construct(int& val) noexcept
        : _val(&val)
    {
        ++(*_val);
    }

    test_construct(const test_construct& orig) noexcept
        : _val(orig._val)
    {
        ++(*_val);
    }

    test_construct& operator=(const test_construct& orig) noexcept
    {
        if(this != &orig)
        {
            _val = orig._val;
            ++(*_val);
        }
        return *this;
    }

    test_construct(test_construct&& orig) noexcept
        : _val(orig._val)
    {
        orig._val = nullptr;
    }

    test_construct& operator=(test_construct&& orig) noexcept
    {
        if(this != &orig)
        {
            _val = orig._val;
            orig._val = nullptr;
        }
        return *this;
    }

    ~test_construct()
    {
        if(_val)
            (*_val)--;
    }
    int* _val;
};

struct test_move
{
    test_move(const char* val) noexcept
        : _val(val)
    {
    }
    test_move(test_move&& orig) noexcept
        : _val(orig._val)
    {
        orig._val = nullptr;
    }
    test_move& operator=(test_move&& orig) noexcept
    {
        if(this != &orig)
        {
            _val = orig._val;
            orig._val = nullptr;
        }
        return *this;
    }

    test_move(const test_move&) = delete;
    test_move& operator=(const test_move&) = delete;

    const char* _val = nullptr;

    bool operator==(const test_move& rval) const noexcept
    {
        return std::equal(_val, _val + strlen(_val), rval._val,
            rval._val + strlen(rval._val));
    }
};

struct test_emplace_struct
{
    int i = 10;
    char c = 'c';
    unsigned int* ui = nullptr;

    test_emplace_struct() noexcept = default;
    test_emplace_struct(const test_emplace_struct&) noexcept = default;
    test_emplace_struct& operator=(const test_emplace_struct&) noexcept = default;
    test_emplace_struct(test_emplace_struct&&) noexcept = default;
    test_emplace_struct& operator=(test_emplace_struct&&) noexcept = default;

    test_emplace_struct(int i_, char c_, unsigned int* ui_)
        : i(i_)
        , c(c_)
        , ui(ui_)
    {
    }

    bool operator==(const test_emplace_struct& rval) noexcept
    {
        return i == rval.i && c == rval.c && ui == rval.ui;
    }
};

#endif //! FIXED_TEST_STRUCT_HPP
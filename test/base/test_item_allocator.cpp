// -*- mode: C++; c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*-
////////////////////////////////////////////////////////////////////
//
// Filename : testitemallocator.cpp
//
// Test of ItemAllocator.
//
// This file is a part of the UPPAAL toolkit.
// Copyright (c) 1995 - 2003, Uppsala University and Aalborg University.
// All right reserved.
//
// $Id: testitemallocator.cpp,v 1.3 2005/05/11 19:08:13 adavid Exp $
//
///////////////////////////////////////////////////////////////////

// Tests are always for debugging.

#ifdef NDEBUG
#undef NDEBUG
#endif

#include "base/ItemAllocator.h"
#include "debug/utils.h"

#include <iostream>
#include <ctime>

using namespace std;
using namespace base;

// Progress counter
#define PROGRESS() debug_spin(stderr)

// test struct
struct test_t
{
    int a, b, c;
};

static void test(int size)
{
    assert(size);
    test_t** t = new test_t*[size];
    for (int k = 0; k < 10; ++k)  // repeat construct/test/destroy
    {
        ItemAllocator<test_t> alloc;
        if (rand() % 2 == 0)
            alloc.reset();           // sometimes reset before use
        for (int j = 0; j < 3; ++j)  // reuse a reset allocator
        {
            int i = 0;
            int nb = 10 * size;
            PROGRESS();
            do {  // fill and empty alloc randomly, more fills
                if (i == 0 || (i < size - 1 && rand() % 4 != 0)) {
                    t[i] = alloc.allocate();
                    // set some magic data
                    t[i]->a = i;
                    t[i]->b = i + 1;
                    t[i]->c = i + 2;
                    i++;
                } else {
                    --i;
                    // check data still there
                    assert(t[i]->a == i);
                    assert(t[i]->b == i + 1);
                    assert(t[i]->c == i + 2);
                    alloc.deallocate(t[i]);
                }
            } while (--nb);
            if (rand() % 2 == 0)  // sometimes empty before reset
            {
                while (i > 0) {
                    --i;
                    assert(t[i]->a == i);
                    assert(t[i]->b == i + 1);
                    assert(t[i]->c == i + 2);
                    alloc.deallocate(t[i]);
                }
            }
            alloc.reset();  // reset alloc
        }
    }
    delete[] t;
}

int main(int argc, char* argv[])
{
    int n, seed;

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " size [seed]\n";
        return 1;
    }

    n = atoi(argv[1]);
    seed = argc > 2 ? atoi(argv[2]) : time(NULL);
    srand(seed);

    /* Print the seed for the random generator
     * to be able to repeat a failed test.
     */
    cout << "Test with seed=" << seed << endl;
    test(n);

    cout << "Passed\n";
    return 0;
}

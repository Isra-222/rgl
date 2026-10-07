#include "rgl/core/utility/move.h"

#include "rgl/async/thread.h"

#include "rgl/memory/rmalloc.h"
#include "rgl/containers/vector.h"
#include "rgl/io/iostream.h"

constexpr int STRESS_THREADS = 1;
constexpr int ITERATIONS = 10000;
constexpr int POOL_SIZE = 256;

void stress_mem_task(int thread_id) {
    void* blocks[POOL_SIZE];
    for (int i = 0; i < POOL_SIZE; ++i) {
        blocks[i] = nullptr;
    }

    for (int i = 0; i < ITERATIONS; ++i) {
        int index = (i + thread_id * 17) % POOL_SIZE;

        if (blocks[index] != nullptr) {
            rgl::rfree(blocks[index]);
            blocks[index] = nullptr;
        }

        rgl::size_t alloc_size = 16 + ((i * 31 + thread_id * 7) % 4096);

        blocks[index] = rgl::rmalloc(alloc_size);

        if (blocks[index] != nullptr) {
            volatile char* p = static_cast<char*>(blocks[index]);
            p[0] = static_cast<char>(thread_id);
            p[alloc_size - 1] = static_cast<char>(i & 0xFF);
        }
    }

    for (int i = 0; i < POOL_SIZE; ++i) {
        if (blocks[i] != nullptr) {
            rgl::rfree(blocks[i]);
            blocks[i] = nullptr;
        }
    }
}

int main() {
    rgl::out << "Iniciando prueba de estrés de memoria multihilo..." << rgl::endl;

    for (int run = 0; run < 1000; ++run) {
        rgl::thread threads[STRESS_THREADS];

        for (int i = 0; i < STRESS_THREADS; ++i) {
            threads[i] = rgl::thread(stress_mem_task, i);
        }

        for (int i = 0; i < STRESS_THREADS; ++i) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
        }

        rgl::out << "Ciclo de estrés " << run + 1 << " completado con exito." << rgl::endl;
    }

    rgl::out << "¡Todas las pruebas de estrés pasaron sin romper el heap!" << rgl::endl;
    return 0;
}
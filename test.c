// use `handle SIGSEGV nostop noprint pass` to disable sigsegv handling in gdb

#include <assert.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

static const int64_t NS_PER_SEC = INT64_C(1000000000);

static int g_num_signals;
static int64_t g_last_sample;

static int64_t time_nanoseconds() {
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC, &tp);
  return (int64_t)tp.tv_sec * NS_PER_SEC + (int64_t)tp.tv_nsec;
}

static void handler(int sig, siginfo_t *si, void *data) {
  g_num_signals++;

  int64_t now = time_nanoseconds();
  int64_t elapsed = now - g_last_sample;

  if (elapsed >= NS_PER_SEC) {
    printf("sigsegv/s %f\n", g_num_signals / (elapsed / (float)NS_PER_SEC));
    g_num_signals = 0;
    g_last_sample = now;
  }
}

int main() {
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = &handler;
  assert(sigaction(SIGSEGV, &sa, NULL) == 0);

  g_last_sample = time_nanoseconds();

  *((char *)NULL) = 0;
}
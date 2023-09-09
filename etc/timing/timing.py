#!/usr/bin/env python3

import argparse
import sys
from collections import defaultdict

NAME = 0
BURST = 1
PRIORITY = 2
ARRIVAL = 3
RUNTIME = 4
IDLE = 'IDLE'

def simulate_execution(processes, scheduler, args, preemptive=False):
    ready = []
    incoming = sorted(processes, key=lambda p: p[ARRIVAL])
    res = []
    time = args.delay
    running = None
    while ready or incoming:
        # Accept incoming processes at this time.
        while incoming and incoming[0][ARRIVAL] <= time:
            ready.append(incoming.pop(0))

        # Apply scheduler algorithm and detect idle process.
        ready = scheduler(ready)
        if not running and ready:
            if time > 0:
                idle = time - sum(p[RUNTIME] for p in res)
                if idle > 0:
                    res.append([IDLE, 0, 0, 0, idle])
            running = ready[0]

        # Preempt running process.
        if preemptive and running and ready and running[NAME] != ready[0][NAME]:
            preempted = running[:]
            res.append(preempted)
            running[BURST] -= running[RUNTIME]
            running[RUNTIME] = 0
            running = ready[0]

        # Advance time.
        time += 1
        if running:
            running[RUNTIME] += 1

        # Run process.
        if running and running[RUNTIME] == running[BURST]:
            ready.remove(running)
            res.append(running)
            running = None

    return res

def schedule_fcfs(processes, args):
    scheduler = lambda ps: sorted(ps, key=lambda p: p[ARRIVAL])
    return simulate_execution(processes, scheduler, args)

def schedule_sjf(processes, args):
    scheduler = lambda ps: sorted(ps, key=lambda p: p[BURST])
    return simulate_execution(processes, scheduler, args)

def schedule_srtf(processes, args):
    scheduler = lambda ps: sorted(ps, key=lambda p: p[BURST])
    return simulate_execution(processes, scheduler, args, preemptive=True)

def schedule_priority(processes, args):
    scheduler = lambda ps: sorted(ps, key=lambda p: p[PRIORITY])
    return simulate_execution(processes, scheduler, args)

def schedule_round_robin(processes, args):
    scheduler = lambda ps: [*ps[1:], ps[0]] if len(ps) > 1 and ps[0][RUNTIME] >= args.quantum else ps
    return simulate_execution(processes, scheduler, args, preemptive=True)

def print_processes(processes):
    headers = [' Process ', ' CPU Burst ', ' Priority ', ' Arrival ']
    table = [
        headers,
        ['-' * len(h) for h in headers],
    ]

    for p in processes:
        table.append([
            ' ' + (x if i == NAME else str(x)).ljust(len(headers[i]) - 1)
            for i, x in enumerate(p[:-1])
        ])

    for row in table:
        print('|', end='')
        for cell in row:
            print(cell, end='|')
        print()

def print_gantt(processes):
    gantt = [[], [], [], []]
    time = 0
    min_block_size = max(len(p[NAME]) for p in processes if p[NAME] != IDLE)

    for p in processes:
        block_size = max(min_block_size, p[RUNTIME]) + 2
        if p[NAME] != IDLE:
            block = f' {p[NAME].ljust(block_size - 2)} '
        else:
            block = '/' * block_size
        line = '-' * block_size
        time += p[RUNTIME]
        gantt[0].append(line)
        gantt[1].append(block)
        gantt[2].append(line)
        gantt[3].append(f'{time}'.rjust(block_size + 1))

    for row in gantt[:-1]:
        print('|', end='')
        for cell in row:
            print(cell, end='|')
        print()

    print('0', end='')
    for row in gantt[-1]:
        for cell in row:
            print(cell, end='')
    print()

def print_metrics(processes):
    waiting_time = defaultdict(int)
    last_runtime = defaultdict(int)
    total_runtime = defaultdict(int)
    arrival = defaultdict(int)
    time = 0
    idle = 0

    for p in processes:
        if p[NAME] == IDLE:
            idle += p[RUNTIME]
        else:
            waiting_time[p[NAME]] = time
        last_runtime[p[NAME]] = p[RUNTIME]
        total_runtime[p[NAME]] += p[RUNTIME]
        arrival[p[NAME]] = p[ARRIVAL]
        time += p[RUNTIME]

    for p_name in waiting_time.keys():
        waiting_time[p_name] -= total_runtime[p_name] - last_runtime[p_name] + arrival[p_name]

    waiting_time = sorted(waiting_time.items(), key=lambda p: p[1])
    avg_waiting_time = sum(wt[1] for wt in waiting_time) / len(waiting_time)

    print('Waiting time')
    for p_name, p_wt in waiting_time:
        print(f'  {p_name}\t= {p_wt}')
    print(f'  MIN\t= {waiting_time[0][NAME]} ({waiting_time[0][1]})')
    print(f'  MAX\t= {waiting_time[-1][NAME]} ({waiting_time[-1][1]})')
    print(f'  AVG\t= {round(avg_waiting_time, 2)}')
    print(f'  IDLE\t= {idle}')

    turnaround_time = sorted([
        (p_name, p_wt + total_runtime[p_name])
        for p_name, p_wt in waiting_time
    ], key=lambda p: p[1])
    avg_turnaround_time = sum(tat[1] for tat in turnaround_time) / len(turnaround_time)

    print('\nTurnaround time')
    for p_name, p_tat in turnaround_time:
        print(f'  {p_name}\t= {p_tat}')
    print(f'  MIN\t= {turnaround_time[0][NAME]} ({turnaround_time[0][1]})')
    print(f'  MAX\t= {turnaround_time[-1][NAME]} ({turnaround_time[-1][1]})')
    print(f'  AVG\t= {round(avg_turnaround_time, 2)}')

def print_title(title):
    if sys.stdout.isatty():
        print(f'\n\033[01m// {title}\033[0m\n')
    else:
        print(f'\n// {title}\n')

class ProcessName(str):
    COLORS = ['\033[31m', '\033[32m', '\033[33m', '\033[34m', '\033[35m', '\033[36m']
    color_reset = '\033[00m'

    def __init__(self, i):
        self.name = f'P{i}'
        if sys.stdout.isatty():
            self.color = self.COLORS[(i-1) % len(self.COLORS)]
            self.extra_len = len(self.color) + len(self.color_reset)
        else:
            self.color = ''
            self.color_reset = ''
            self.extra_len = 0

    def __len__(self):
        return len(self.name)

    def __str__(self):
        return f'{self.color}{self.name}{self.color_reset}'

    def ljust(self, width, fillchar=' '):
        return str(self).ljust(width + self.extra_len, fillchar)

    def rjust(self, width, fillchar=' '):
        return str(self).rjust(width + self.extra_len, fillchar)

def main():
    parser = argparse.ArgumentParser(description='Scheduler Timing Tool')
    parser.add_argument('processes', metavar='burst priority arrival', type=int, nargs='+', help='burst, priority and arrival for each process')
    parser.add_argument('-q', '--quantum', action='store', type=int, default=1, help='quantum duration')
    parser.add_argument('-d', '--delay', action='store', type=int, default=0, help='initial scheduler delay')
    parser.add_argument('-s', '--scheduler', metavar='ALGO', action='append', help='scheduler algorithm (FCFS, SJF, Priority, Round-Robin)')
    args = parser.parse_args()

    if not args.processes or len(args.processes) % 3 != 0:
        parser.print_help()
        sys.exit(1)

    processes = [
        [ProcessName(i//3+1), *args.processes[i:i+3], 0]
        for i in range(0, len(args.processes), 3)
    ]

    print_processes(processes)

    schedulers = [
        ['FCFS', '(First come, first served)', schedule_fcfs],
        ['SJF', '(Shortest job first / non-preemptive)', schedule_sjf],
        ['SRTF', '(Shortest remaining time first / preemptive SJF)', schedule_srtf],
        ['Priority', '(non-preemptive)', schedule_priority],
        ['Round-Robin', f'(quantum = {args.quantum})', schedule_round_robin],
    ]

    args.scheduler = [s.lower() for s in args.scheduler] if args.scheduler else []
    for s_name, s_desc, s_func in schedulers:
        if args.scheduler and s_name.lower() not in args.scheduler: continue
        res = s_func([p[:] for p in processes], args)
        print_title(f'{s_name} {s_desc}')
        print_gantt(res)
        print()
        print_metrics(res)

if __name__ == '__main__':
    main()

# Fitness Monitor v2 Overview
## Structure
This file outlines the structure and architecture of the version two fitness monitor project. It extends version one, summarised in [fitness-monitor-v1-overview](fitness-monitor-v1-overview.md). The version two fitness monitor implements a significantly refined structure. This refactor was performed to port the fitness monitor to use a Real-Time Operating System (RTOS) as required for this project. This implementation is detailed in [rtos-implementation](./RTOS-implementation.md). The specifc architecture is explained and justified in [architecture-justification](./architecture-justification.md)
![Key details of fitness monitor v2 source structure](./images/version2-structure.svg)

## Scope
The v2 fitness monitor implements the specifications described in [fitness-monior-v2-spec](./fitness-monitor-v2-spec.md), which extends the requirements of the v1 firmware. There are several key features added, including improved step counting, configurable user parameters for more accurate distance estimation, energy estimation, a configurable inactivity prompt, and a stopwatch. These features are described in depth in [fitness-monitor-v2-features](./fitness-monitor-v2-features.md).

## Future work
Although version two has made significant progress on improving the state of the project from a software design perspective, there are a few identified areas of improvement that have not been addressed. These are

- Decoupling display interaction into its own thread separate to the device state.
- Correctly implementing a display hardware abstraction layer to support any display interface.
- Improving the input queue interface.

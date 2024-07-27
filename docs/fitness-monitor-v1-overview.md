# Fitness Monitor v1 Overview

This file documents the structure and implementation of the fitness monitor v1.
To allow for a compirison with the new implementation for version 2.

## Structure

The version 1 firmware has a modularized structure with several layers of
abstraction. An overview of the modules can be seen in Figure 1. The key
take aways from this diagram are several modules with simular names and
the circular dependancy created by the use of deviceStateInfo_t.

![Fitness Monitor v1 source structure](./images/version1-structurev2.png)

Figure 1: Fitness Monitor v1 source structure.

### Modules

A more in depth description of the version 1's modules and there functions can be found below:

- `button_manager`
- `display_manager`
- `adc_read`
- `accl_manger`
- `circBufV/T`
- `serial_sender`
- `button_4`
- `switches`
- `I2C_driver`

## Architecture

The fitness monitor version 1 uses a ----.

## Evaluation


## Step Counting

The step counting algorithm used by fitness monitor version 1 is detailed
as follows.

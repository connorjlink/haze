# HAZE
> A bespoke, from-scratch, full-stack toolchain for my new processor architecture, Haze.

## MAIN FEATURES
- Fully custom assembly language and assembler
- Fully custom compiler
  - Haze CPU support
  - x86_64 CPU support
- Fully custom interpreter
  - Function as `geo` engine scripting language
    - Use custom MQTT-based RPC
- Custom CPU implementation in SystemVerilog

## INTERPRETED FIBONACCI PROGRAM
![Fibonacci Program](resources/example.png)


## CONNECTED PROJECTS
- _haze-ls_, my fully LSP-compliant language server featuring contextual autocompletion and custom syntax highlighting
  - [https://github.com/connorjlink/haze-ls](https://github.com/connorjlink/haze-ls)
  - [https://marketplace.visualstudio.com/items?itemName=connorjlink.haze-ls](https://marketplace.visualstudio.com/items?itemName=connorjlink.haze-ls)
- _geo_, my custom 3-D voxel rendering engine with which interpreted Haze can interact
  - [https://github.com/connorjlink/geo](https://github.com/connorjlink/geo)
- _inspector_, my custom C#/WPF tool to monitor interpreter MQTT traffic
  - [https://github.com/connorjlink/inspector](https://github.com/connorjlink/inspector)

onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -lib xil_defaultlib Jay_opt

do {wave.do}

view wave
view structure
view signals

do {Jay.udo}

run -all

quit -force

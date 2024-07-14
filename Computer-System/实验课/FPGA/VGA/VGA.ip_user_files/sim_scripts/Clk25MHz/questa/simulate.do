onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -lib xil_defaultlib Clk25MHz_opt

do {wave.do}

view wave
view structure
view signals

do {Clk25MHz.udo}

run -all

quit -force

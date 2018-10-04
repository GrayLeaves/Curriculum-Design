@echo off
"..\..\bin\caffe.exe" train --solver=solver.prototxt --gpu=1 --snapshot=models/_iter_92932.solverstate
pause
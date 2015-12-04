..\xasm tst6502
if errorlevel 1 goto ex
diff tst6502.hex tst6502.sav
if errorlevel 1 goto ex
diff 6502.lst 6502.sav
:ex

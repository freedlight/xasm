..\xasm tst8051
if errorlevel 1 goto ex
diff tst8051.hex tst8051.sav
if errorlevel 1 goto ex
diff 8051.lst 8051.sav
:ex

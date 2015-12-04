..\xasm tst65c02
if errorlevel 1 goto ex
diff tst65c02.hex tst65c02.sav
if errorlevel 1 goto ex
diff 65c02.lst 65c02.sav
:ex

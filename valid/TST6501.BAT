..\xasm tst6501
if errorlevel 1 goto ex
diff tst6501.hex tst6501.sav
if errorlevel 1 goto ex
diff 6501.lst 6501.sav
:ex

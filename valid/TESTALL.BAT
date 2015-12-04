echo off
cls
call tst6501
if errorlevel 1 goto bad
cls
call tst6502
if errorlevel 1 goto bad
cls
call tst65c00
if errorlevel 1 goto bad
cls
call tst65c02
if errorlevel 1 goto bad
cls
call tst6803
if errorlevel 1 goto bad
cls
call tst6805
if errorlevel 1 goto bad
cls
call tst6811
if errorlevel 1 goto bad
cls
call tst8048
if errorlevel 1 goto bad
cls
call tst8051
if errorlevel 1 goto bad
cls
call tst8080
if errorlevel 1 goto bad
cls
call tst8085
if errorlevel 1 goto bad
cls
echo xasm test passed
echo 
echo 
goto ex
:bad
echo xasm test failed
:ex

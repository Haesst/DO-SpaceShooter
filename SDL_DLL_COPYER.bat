@echo off

::List of dll files to copy
::Main SDL2
set list=ThirdParty\SDL2\lib\x86\SDL2.dll
::SDL2 image
set list=%list%;ThirdParty\SDL2_image\lib\x86\SDL2_image.dll
set list=%list%;ThirdParty\SDL2_image\lib\x86\libwebp-7.dll
set list=%list%;ThirdParty\SDL2_image\lib\x86\libtiff-5.dll
set list=%list%;ThirdParty\SDL2_image\lib\x86\libpng16-16.dll
set list=%list%;ThirdParty\SDL2_image\lib\x86\libjpeg-9.dll
::SDL2 tff
set list=%list%;ThirdParty\SDL2_ttf\lib\x86\libfreetype-6.dll
set list=%list%;ThirdParty\SDL2_ttf\lib\x86\SDL2_ttf.dll
::Shared between SDL2 image and ttf
set list=%list%;ThirdParty\SDL2_image\lib\x86\zlib1.dll

for %%a in (%list%) do ( 
 copy /y %%a Debug
)

pause
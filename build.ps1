$localEmxx = Join-Path $PSScriptRoot ".omx\emsdk\upstream\emscripten\em++.exe"
$emxx = if (Test-Path $localEmxx) { $localEmxx } else { (Get-Command em++ -ErrorAction Stop).Source }
$tempDir = Join-Path $PSScriptRoot ".omx\temp"
New-Item -ItemType Directory -Force $tempDir | Out-Null
$env:TEMP = $tempDir
$env:TMP = $tempDir

& $emxx `
  "Watch List Film.cpp" `
  -std=c++17 `
  -O3 `
  -sASYNCIFY `
  -sMODULARIZE `
  -sEXPORT_ES6 `
  -sINVOKE_RUN=0 `
  -sEXIT_RUNTIME=1 `
  "-sEXPORTED_FUNCTIONS=_main,_malloc,_free" `
  "-sEXPORTED_RUNTIME_METHODS=ccall" `
  -o watchlist.mjs

exit $LASTEXITCODE

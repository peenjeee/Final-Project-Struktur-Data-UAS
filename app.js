import createWatchlist from "./watchlist.mjs";
import { Terminal } from "./vendor/xterm/xterm.mjs";
import { FitAddon } from "./vendor/xterm/addon-fit.mjs";
import hljs from "./vendor/highlight/core.min.js";
import cpp from "./vendor/highlight/cpp.min.js";

hljs.registerLanguage("cpp", cpp);

const terminalElement = document.querySelector("#terminal");
const runButton = document.querySelector("#run");
const clearButton = document.querySelector("#clear");
const sourceCode = document.querySelector("#source-code");
const copyButton = document.querySelector("#copy-code");
const backToTop = document.querySelector("#back-to-top");

let resolveInput;
let currentInput = "";

const styles = getComputedStyle(document.documentElement);
const color = (name) => styles.getPropertyValue(name).trim();
const terminal = new Terminal({
  convertEol: true,
  cursorBlink: true,
  fontFamily: '"Geist Mono", Consolas, monospace',
  fontSize: 15,
  lineHeight: 1.35,
  scrollback: 2000,
  theme: {
    background: color("--color-surface-base"),
    foreground: color("--color-text-inverse"),
    cursor: color("--color-text-tertiary"),
    selectionBackground: color("--color-text-primary"),
    blue: color("--color-text-primary"),
    brightBlue: color("--color-text-primary"),
    green: color("--color-text-tertiary"),
    brightGreen: color("--color-text-tertiary")
  }
});
const fitAddon = new FitAddon();
terminal.loadAddon(fitAddon);
terminal.open(terminalElement);
requestAnimationFrame(() => fitAddon.fit());
document.fonts.ready.then(() => fitAddon.fit());
new ResizeObserver(() => fitAddon.fit()).observe(terminalElement);

window.addEventListener("scroll", () => {
  backToTop.hidden = window.scrollY < 600;
}, { passive: true });

backToTop.addEventListener("click", () => {
  window.scrollTo({ top: 0, behavior: "smooth" });
});

fetch("./Watch%20List%20Film.cpp")
  .then((response) => {
    if (!response.ok) throw new Error(`HTTP ${response.status}`);
    return response.text();
  })
  .then((code) => {
    sourceCode.innerHTML = hljs.highlight(code, { language: "cpp", ignoreIllegals: true }).value;
    sourceCode.classList.add("hljs", "language-cpp");
  })
  .catch((error) => { sourceCode.textContent = `Source code gagal dimuat: ${error.message}`; });

async function copyText(text) {
  try {
    if (navigator.clipboard?.writeText) {
      await navigator.clipboard.writeText(text);
      return;
    }
  } catch {}

  const textarea = document.createElement("textarea");
  textarea.value = text;
  textarea.setAttribute("readonly", "");
  textarea.style.position = "fixed";
  textarea.style.opacity = "0";
  document.body.append(textarea);
  textarea.focus();
  textarea.select();
  const copied = document.execCommand("copy");
  textarea.remove();
  if (!copied) throw new Error("Copy gagal");
}

copyButton.addEventListener("click", async () => {
  try {
    await copyText(sourceCode.textContent);
    copyButton.classList.add("copied");
    copyButton.setAttribute("aria-label", "Kode tersalin");
    copyButton.title = "Kode tersalin";
  } catch {
    copyButton.setAttribute("aria-label", "Gagal menyalin kode");
    copyButton.title = "Gagal menyalin kode";
  }
  setTimeout(() => {
    copyButton.classList.remove("copied");
    copyButton.setAttribute("aria-label", "Salin kode");
    copyButton.title = "Salin kode";
  }, 1600);
});

function append(text, kind = "") {
  const ansi = kind === "error" ? "\x1b[1;34m" : "";
  terminal.writeln(`${ansi}${text}\x1b[0m`);
}

function clearTerminal() {
  terminal.reset();
  terminal.clear();
}

globalThis.readTerminalLine = () => new Promise((resolve) => {
  resolveInput = resolve;
  currentInput = "";
  terminal.write("\x1b[1;32m› \x1b[0m");
  terminal.focus();
});

terminal.onData((data) => {
  if (!resolveInput || data.startsWith("\x1b")) return;

  for (const char of data) {
    if (char === "\r" || char === "\n") {
      terminal.write("\r\n");
      const resolve = resolveInput;
      resolveInput = undefined;
      resolve(currentInput);
      currentInput = "";
      break;
    }
    if (char === "\x7f") {
      if (currentInput) {
        currentInput = currentInput.slice(0, -1);
        terminal.write("\b \b");
      }
      continue;
    }
    if (char >= " ") {
      currentInput += char;
      terminal.write(char);
    }
  }
});

function showWelcome() {
  clearTerminal();
  terminal.writeln("\x1b[1;32mWatch List Film C++ Terminal\x1b[0m");
  terminal.writeln("\x1b[34m$\x1b[0m Tekan tombol Jalankan untuk memulai program.");
}

showWelcome();

runButton.addEventListener("click", async () => {
  clearTerminal();
  terminal.writeln("\x1b[1;32m$ ./watch-list-film\x1b[0m");
  runButton.disabled = true;
  clearButton.disabled = true;

  try {
    const program = await createWatchlist({
      print: (text) => append(text),
      printErr: (text) => append(text, "error")
    });
    await program.ccall("main", "number", [], [], { async: true });
    append("Program selesai.");
  } catch (error) {
    append(`Program berhenti: ${error.message ?? error}`, "error");
  } finally {
    resolveInput = undefined;
    currentInput = "";
    runButton.disabled = false;
    clearButton.disabled = false;
  }
});

clearButton.addEventListener("click", showWelcome);

# FileClip

**fileclip** is a simple command-line clipboard tool for files and directories on macOS. It lets you "copy" a file or folder to the clipboard and "paste" it later into the current directory — like a CLI-based copy-paste for the file system.

📋 Built on macOS clipboard utilities (`pbcopy` and `pbpaste`)  
🧠 Remembers absolute file paths using your system clipboard  
🛠 Great for scripts, terminal workflows, or keyboard shortcuts

---

## 🚀 Features

- Copy files or directories to clipboard (`-c`)
- Paste to current working directory (`-p`)
- Show current clipboard contents (`-d`)
- Clear clipboard (`-r`)
- Built specifically for macOS

---

## 🛠 Installation

### Using Make

```bash
git clone https://github.com/yourusername/fileclip.git
cd fileclip
./install.sh
```

This builds and installs the `fileclip` binary to `/usr/local/bin`.

---

## 🧪 Usage

```bash
fileclip -c somefile.txt      # Copy file
fileclip -c somedir           # Copy folder
fileclip -c                   # Copy current directory
fileclip -p                   # Paste into current folder
fileclip -d                   # Show clipboard contents
fileclip -r                   # Clear clipboard
fileclip -v                   # Show version
fileclip -h                   # Help
```

### Example

```bash
cd ~/Documents
fileclip -c notes.txt         # Copy full path to notes.txt

cd ~/Desktop
fileclip -p                   # Paste notes.txt to Desktop
```

---

## 🧰 Developer

### Build manually

```bash
make
./fileclip -h
```

### Uninstall

```bash
sudo make uninstall
```

---

## 🍎 macOS Requirements

- macOS with `pbcopy` and `pbpaste` (standard in macOS)
- `clang` or `gcc` to build (comes with Xcode CLI tools)

---

## 📝 License

MIT License © 2025 Gregory Navasarkian

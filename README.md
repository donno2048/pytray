# pytray

A Python module to interact with the Windows tray.

## Installation

### From PyPI

```sh
pip3 install py-tray
```

### From GitHub

```sh
pip3 install git+https://github.com/donno2048/pytray
```

## Usage

Here is a simple example:

```py
import pytray, time, os
pytray.create_tray_element()
pytray.add_icon()
i = 0
while True:
    time.sleep(.1)
    pytray.set_icon(os.path.abspath(f"cat\\{i}.ico"))
    i = (i + 1) % 5
```

In the future, I will add more features you can see in main.py.

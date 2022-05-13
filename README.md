# pytray

A Python module to interact with the Windows tray.

## Installation

### From PyPI

```sh
pip3 install ptray
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
pytray.create_menu(
    ("Exit", 1), ("Close", 2)
)
pytray.add_icon()
i = 0
while True:
    start_time = time.time()
    while time.time() - start_time < 0.3:
        click = pytray.what_is_clicked()
        if click == 1:
            exit()
        elif click == 2:
            pytray.hide_menu()
        elif click == None:
            pass
        time.sleep(0.1) # make sure only one click is registered
    pytray.set_icon(os.path.abspath(f"cat\\{i}.ico"))
    i = (i + 1) % 5
```

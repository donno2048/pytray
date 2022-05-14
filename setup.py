from setuptools import setup, Extension
setup(
    name='ptray',
    version='1.1.2',
    license='MIT',
    author='Elisha Hollander',
    author_email='just4now666666@gmail.com',
    description='A Python module to interact with the Windows tray.',
    long_description=open('README.md').read(),
    long_description_content_type="text/markdown",
    url='https://github.com/donno2048/pytray',
    project_urls={
        'Documentation': 'https://github.com/donno2048/pytray#readme',
        'Bug Reports': 'https://github.com/donno2048/pytray/issues',
        'Source Code': 'https://github.com/donno2048/pytray'
    },
    ext_modules=[Extension('pytray', ['pytray.c'], libraries = ["user32", "shell32"])],
    classifiers=[
        "Environment :: Win32 (MS Windows)",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3"
    ],
    zip_safe = False,
    platforms='win32'
)
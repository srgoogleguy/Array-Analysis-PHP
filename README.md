Array Analysis Tool for PHP
===========================

This is a PHP extension meant to provide internal data structure analysis capabilities for the PHP `Array` data type.
The extension currently is **experimental** and is not meant for production use.

Usage
-----

There is currently only one function provided by this extension: `array_size`

The `array_size` function expects an array as the first argument, with an optional second `boolean` argument which
when `true` will output debug information about the array's internal structure, including a break down of the memory
allocation units and hash collision information. The function returns an `integer` representing the total memory
consumed in bytes for the array supplied as the first argument.

The debug information can be used to determine where hash collisions arise in the array, but currently this is useless
in its present form as the function does not return this information directly to user space.

The function works recursively so the total sum of all the array's parts and dimensions will be accounted for.

Example
-------

    $arr = array(1,2,3);
    var_dump(array_size($arr));

    /* Output */

    int(448)


Installation Instructions
-------------------------

- Unpack the tarball into a directory of choice (e.g. `/usr/local/src/`) and run `phpize`.
- Run `./configure`
- Run `make` and `make install`
- Edit your `php.ini` by adding the line `extension=aa.so` at the end
- Restart your PHP and enjoy


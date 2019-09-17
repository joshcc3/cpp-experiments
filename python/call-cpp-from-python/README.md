This guide works straight out of the box: https://docs.python.org/3/extending/extending.html

You create a module according to the guide above (create a module object to return within the module creation hook - the module object will contain info on the module functions, functions accept self, and a tuple of the arguments).
Then you build a dynamic lib of it using setuptools distutil, this will usually build it in build/. Add the root containing the dynamic lib to your python path and now you should be able to access your module from python scripts.


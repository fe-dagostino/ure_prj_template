# URE Template 

A template repository with a preconfigured **CMakeLists.txt** and a minimal program.

## Customization

In CMakeLists.txt replace value *"my-prj"*, with the name of your project.

```
set(prjname "my-prj")
```

Immediately after in the following sections, will be possible to replace your "project description".

```
# Set the project name and language
project( ${prjname}
         VERSION 0.1.0
         DESCRIPTION "Project Template"
         LANGUAGES CXX C
)
```

## Important

Resources both media and shaders are loaded using relative paths, so take care to have "resources" sub-folder at the same level of binary file. 

Eg. Supposing to have our binary file (*prj*) in a sub-folder named *build* we can execute from the project folder with a call like:

```
#./build/prj
```

or we can move in *build*, create a link to resources and then execute the execute the program:

```
#cd build
#ln -s ../resources
#./prj
```
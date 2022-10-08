# SOLUCIÓN PLANTILLA VSC++ PARA PROYECTOS DE ISGI/ETSINF/UPV

## Copia y uso en ámbitos docentes

### Objetivo

Este repositorio sirve como plantilla de una solución de Visual Studio para construir proyectos con OpenGL y GLUT con fines docentes exclusivamente.

### Instalación

Para instalar, hay que *clonar* el proyecto usando *git* en la consola (p.e. *bash*) sobre el directorio preferido:
```
$ cd $USER/source/repos   
$ git clone -b main --recursive https://github.com/RobVivo/Pracicas_ISGI
```

### Uso

:one: Encaminar la carga de bibliotecas dinámicas `*.dll` a `codebase\bin` en la variable de entorno *path* de la cuenta del usuario  
:two: Abrir Visual Studio y cargar la solución Practicas_ISGI.sln  
:three: Crear un proyecto vacío, seleccionar la plataforma `Release|x86` y cargar la hoja de propiedades *GLPropertySheet.props*   

La solución incluye un proyecto *Practica_00* compilado de ejemplo que debe poder ejecutarse sin problemas si la instalación es correcta.

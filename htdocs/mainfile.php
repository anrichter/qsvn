<?php

/***************************************************************************
 *   This file is part of QSvn Project http://qsvn.berlios.de              *
 *   Copyright (c) 2004-2005 Andreas Richter <ar@oszine.de>                *
/***************************************************************************/


function htmlhead() {
  printf('<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">');
  printf('<html>');
  printf('<head>');
  printf('<meta name="keywords" content="');
    printf('qsvn, subversion, client, qt');
  printf('">');
  printf('<meta name="description" content="QSvn - a Qt based Subversion Client">');
  printf('<title>QSvn - a Qt based Subversion Client</title>');
  printf('<meta name="robots" content="follow">');
  printf('<link rel=stylesheet type="text/css" href="screen.css">');
  printf('</head>');
  printf('<body>');
}

function htmlfoot() {
  printf('</body>');
  printf('</html>');
}

function pagehead() {
  echo "
  <div align=\"center\">
  ";
}

function pagefoot() {
  echo "
  </div>
  ";
}

?>

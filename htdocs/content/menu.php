<?php

/***************************************************************************
 *   This file is part of QSvn Project http://qsvn.berlios.de              *
 *   Copyright (c) 2004-2005 Andreas Richter <ar@oszine.de>                *
/***************************************************************************/


function printmenu($action) {
  printf('<ul id="mainmenu" class="navigation">');

  printf('<li class="main"><a href="?action=start">Index</a></li>');

  if (strstr("project about team", $action)) {
    printf('<li class="main">Project</li>');
    printf('<li><a href="?action=about">About</a></li>');
    printf('<li><a href="?action=team">Team</a></li>');
  } else {
    printf('<li class="main"><a href="?action=project">Project</a></li>');
  }

  if (strstr("screenshots_0.1.0", $action)) {
    printf('<li class="main">Screenshots</li>');
    printf('<li><a href="?action=screenshots_0.1.0">QSvn 0.1.0</a></li>');
  } else {
    printf('<li class="main"><a href="?action=screenshots">Screenshots</a></li>');
  }

  printf('</ul>');
}

?>


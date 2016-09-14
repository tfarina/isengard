<?php

/**
 * Try to get real IP from client web request
 *
 * @return string
 */
function get_client_ip() {
  if (!empty($_SERVER["HTTP_CLIENT_IP"])) {
    return $_SERVER["HTTP_CLIENT_IP"];
  } elseif (!empty($_SERVER["HTTP_X_FORWARDED_FOR"])) {
    return $_SERVER["HTTP_X_FORWARDED_FOR"];
  } elseif (!empty($_SERVER['HTTP_X_FORWARDED'])) { 
    return $_SERVER["HTTP_X_FORWARDED"];
  } elseif (!empty($_SERVER["HTTP_FORWARDED_FOR"])) {
    return $_SERVER["HTTP_FORWARDED_FOR"];
  } elseif (!empty($_SERVER['HTTP_FORWARDED'])) { 
    return $_SERVER["HTTP_FORWARDED"];
  } else {
    return $_SERVER["REMOTE_ADDR"];
  }
}

?>

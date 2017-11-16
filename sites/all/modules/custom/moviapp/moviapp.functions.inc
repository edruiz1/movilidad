<?php

function _add_moviapp($table,$fields){
  db_set_active('moviapp');
  $id = db_insert($table) 
  ->fields($fields)
  ->execute();

  db_set_active();

  return $id;

}

function _edit_moviapp($table,$fields,$condition) {
  db_set_active('moviapp');

  $id = db_update($table) 
  ->fields($fields)
  ->condition($condition['field'],$condition['value'],'=')
  ->execute();

  db_set_active();

  return $id;

}

function _get_by_id_moviapp($table,$condition){
  db_set_active('moviapp');

  $result = db_select($table,'t') 
  ->fields('t')
  ->condition($condition['field'],$condition['value'],'=')
  ->execute()
  ->fetchAssoc();

  db_set_active();

  return $result;

}



function _get_options_secretaria(){
  db_set_active('moviapp');

  $result = db_select('movcom_secretarias','t') 
  ->fields('t')
  ->execute()
  ->fetchAll();

  db_set_active();
  $options = array('Seleccionar');
  foreach ($result as $secretaria) {
    $options[$secretaria->secretaria_id] = $secretaria->secretaria_nombre;
  }

  //drupal_set_message('<pre>'.print_r($options,true).'</pre>');


  return $options;

}

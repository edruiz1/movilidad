<?php
/**
 * @file
 */
module_load_include('inc', 'moviapp', 'moviapp.functions');
function moviapp_dominio_form($form, &$form_state, $type = NULL,$id = NULL) {

  $form = array();
  if (!is_null($id)){
      switch ($type) {
        case 'dominio':
            $dominio = _get_by_id_moviapp('movcom_dominios',array('field'=>'dominio_id','value'=>$id));
            $form_state['values']['dominio_nombre'] = $dominio['dominio_nombre'];
            $form_state['values']['dominio_estado'] = $dominio['dominio_estado'];
          break;
        case 'secretaria':
            $dominio = _get_by_id_moviapp('movcom_secretarias',array('field'=>'secretaria_id','value'=>$id));
            $form_state['values']['secretaria_nombre'] = $dominio['secretaria_nombre'];
            $form_state['values']['secretaria_descripcion'] = $dominio['secretaria_descripcion'];
          break;
        case 'entidad':
            $dominio = _get_by_id_moviapp('movcom_entidades',array('field'=>'entidad_id','value'=>$id));
            $form_state['values']['entidad_nombre'] = $dominio['entidad_nombre'];
            $form_state['values']['entidad_descripcion'] = $dominio['entidad_descripcion'];
            $form_state['values']['secretaria_id'] = $dominio['secretaria_id'];
          break;
      }
  }

  $form['id'] = array (
    '#type'=>'hidden',
    '#value' => $id
    );
  $form['tipo'] = array (
    '#type'=>'hidden',
    '#value' => $type
    );

  $form[$type] = array (
     '#type'        => 'fieldset',
     '#tree'        => true, 
  );


  $form[$type][$type.'_nombre'] = array (
     '#type'        => 'textfield',
     '#title'       => t('Nombre'),
     '#description' => t('Digite nombre del dominio ej: test.com'),
     '#required'    => true,
     '#default_value' => isset($form_state['values'][$type.'_nombre'])?$form_state['values'][$type.'_nombre']:'',
  );

  switch ($type) {
    case 'dominio':
        $form[$type][$type.'_estado'] = array (
           '#type'        => 'radios',
           '#title'       => t('Estado'),
           '#required'    => true,
           '#options'     => array(0=>'Desactivado',1=>'Activado'),
           '#default_value' => isset($form_state['values'][$type.'_estado'])?$form_state['values'][$type.'_estado']:1,
        );
      break;
    case 'secretaria':
    case 'entidad':
        $form[$type][$type.'_descripcion'] = array (
           '#type'        => 'textfield',
           '#title'       => t('Descripcion'),
           '#required'    => true,
           '#default_value' => isset($form_state['values'][$type.'_descripcion'])?$form_state['values'][$type.'_descripcion']:'',
        );
        if ($type == 'entidad'){
            $form[$type]['secretaria_id'] = array (
               '#type'        => 'select',
               '#title'       => t('Secretaria'),
               '#required'    => true,
               '#options'     => _get_options_secretaria(),
               '#default_value' => isset($form_state['values']['secretaria_id'])?$form_state['values']['secretaria_id']:0,
            );

        }
      break;
  }



  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Grabar'),
    );

  return $form;
}


function moviapp_dominio_form_submit($form, &$form_state) {
  $tipo   = $form_state['values']['tipo'];
  $values = $form_state['values'][$tipo];
  switch ($tipo) {
    case 'dominio':
       $table    = 'movcom_dominios';
       $field_id = 'dominio_id';
      break;
    case 'secretaria':
       $table = 'movcom_secretarias';
       $field_id = 'secretaria_id';
      break;
    case 'entidad':
       $table = 'movcom_entidades';
       $field_id = 'entidad_id';
      break;
    
  }

  if (isset($form_state['values']['id']) && 
      !empty($form_state['values']['id']) && 
      $form_state['values'] > 0){
      $id = _edit_moviapp($table,$values,array('field'=>$field_id,'value'=>$form_state['values']['id']));
  }
  else {
      $id = _add_moviapp($table,$values);  
  }
}

function moviapp_list_page($type){
   $result = _get_all_moviapp('movcom_'.$type);
   $rows   = array();
   $headers = array();
   $flag_first = true;
   foreach ($result as $value) {
       $row = (array)$value;
       if ($type == 'entidades'){
          unset($row['secretaria_id']);
       }
       if ($flag_first){
           $flag_first = FALSE;
           foreach ($row as $key => $field) {
             $headers[$key] = $key;
           }
           $headers['editar'] = 'Editar';

       }
       switch ($type) {
         case 'dominios':
            $row['editar'] = '<a href="/movilidad/movcom/dominio/'.$row['dominio_id'].'/editar">Editar</a>';
            if ($row['dominio_estado'] == 1){
               $row['dominio_estado'] = 'SI';
            }
            else {
               $row['dominio_estado'] = 'NO';
            }
           break;
         case 'secretarias':
            $row['editar'] = '<a href="/movilidad/movcom/secretaria/'.$row['secretaria_id'].'/editar">Editar</a>';
           break;
         case 'entidades':
            $row['editar'] = '<a href="/movilidad/movcom/entidad/'.$row['entidad_id'].'/editar">Editar</a>';
           break;
       }
       $rows[] = $row;
   }

   switch ($type) {
     case 'dominios':
        $buttom = '<a class="btn btn-success" href="/movilidad/movcom/dominio/crear">Crear Dominio</a>';
       break;
     case 'secretarias':
        $buttom = '<a class="btn btn-success" href="/movilidad/movcom/secretaria/crear">Crear Secretaria</a>';
       break;
     case 'entidades':
        $buttom = '<a class="btn btn-success" href="/movilidad/movcom/entidad/crear">Crear Entidad</a>';
       break;
   }

   $per_page = 10;
   $current_page = pager_default_initialize(count($rows), $per_page);
   $chunks  = array_chunk($rows, $per_page, TRUE);   
   $output  = theme('table', array('header' => $headers, 'rows' => $chunks[$current_page])). $buttom;
   $output .= theme('pager', array('quantity',count($rows))); 
   return $output;
}

function moviapp_reports($type){
   $result = _get_report_moviapp($type);
   $rows   = array();
   $headers = array();
   $flag_first = true;
   foreach ($result as $value) {
       $row = (array)$value;
       if ($type == 'entidades'){
          unset($row['secretaria_id']);
       }
       if ($flag_first){
           $flag_first = FALSE;
           foreach ($row as $key => $field) {
             $headers[$key] = $key;
           }

       }
       $rows[] = $row;
   }
   $per_page = 10;
   $current_page = pager_default_initialize(count($rows), $per_page);
   $chunks  = array_chunk($rows, $per_page, TRUE);   
   $form = drupal_get_form('moviapp_report_form',$type);
   $output  = drupal_render($form);
   $output .= theme('table', array('header' => $headers, 'rows' => $chunks[$current_page]));
   $output .= theme('pager', array('quantity',count($rows))); 
   return $output;
}

function moviapp_report_form($form, &$form_state,$type=NULL) {


  $options = array('Selecionar','Usuarios registrados','Vehiculos registrados','Rutas registradas','Biciviajes','Sitios favoritos');
    $form = array();
    $form['type'] = array(
      '#type'=>'select',
      '#title' => 'Reporte',
      '#description '=> 'Seleccione el reporte a generar',
      '#options'=>$options,
      '#required' => true,
      '#default_value' => $type,
      );

    $form['generar']=array(
        '#type'=>'submit',
        '#value'=>'Generar'
      );

    return $form;
  }

function moviapp_report_form_submit($form, &$form_state) {
     drupal_goto('/movcom/reportes/'.$form_state['values']['type']);
}
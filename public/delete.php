<?php      
$status=unlink($_POST['file_name']);    
if($status){  
echo "<h1>File deleted successfully</h1>";    
}else{  
echo "<h1>NOP!</h1>";    
}  
?>  
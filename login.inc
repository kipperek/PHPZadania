<?php
class LogIn{
	
		function manageUser(){
			$pass_invalid = false;
			
			if(isset($_POST['user_login']) && isset($_POST['user_pass'])){
				$base = new DataBase();
				if(!$base->getUser($_POST['user_login'],$_POST['user_pass']))
					$pass_invalid = true;
				
				unset($_POST['user_login']);
				unset($_POST['user_pass']);
			}
			
			if(isset($_POST['logout'])){
				unset($_SESSION['user_id']);
				unset($_SESSION['user_login']);
				unset($_POST['logout']);
			}
			
			if(isset($_SESSION['user_id']) && isset($_SESSION['user_login']))
				$this->printInfo($_SESSION['user_login']);
			else{
				$this->printForm();
				if($pass_invalid)
					echo "<div class='form_error'>Hasło lub login jest nieprawidłowe.</div>";
			}
		}
		
		function printInfo($name){
			echo "<div id='loggedDiv'><form method='post' action='index.php";
			if(isset($_GET['kat_id']))
				echo "?kat_id=".$_GET['kat_id'];
			echo"'>Zalogowany jako <b>".$name."</b>. <input type='submit' name='logout' value='Wyloguj'></form></div>";
		}
		
		function printForm(){
			echo "<div id='loginDiv'><form method='post' action='index.php";
			if(isset($_GET['kat_id']))
				echo "?kat_id=".$_GET['kat_id'];
			echo "'>";
			echo "<b>Login:</b> <input type='text' name='user_login' class='login'> <b>Hasło:</b> <input type='password' name='user_pass' class='login'>";
			echo "<input type='submit' value='Zaloguj'></form></div>";
		}
	}
?>
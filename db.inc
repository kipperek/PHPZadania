<?php
		class DataBase{
		private $db;
		private $conn = 'mysql:host=localhost;dbname=kacperczechowi_z;charset=utf8';
		private $base = '****';
		private $pass = '****';
		
		function __construct(){
			try{
				$this->db = new PDO($this->conn,$this->base,$this->pass);
				$this->db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
			}catch(PDOException $e){
				echo $e->getMessage();
			}
		}
		
		function getUser($login, $pass){
			$result = false;
			try{
				$login = str_replace(array("-",'"',"'","/","\\"),"",$login);
				$pass = str_replace(array("-",'"',"'","/","\\"),"",$pass);
				
				$stmt = $this->db->prepare("SELECT id_user, login FROM user WHERE login=:log AND haslo=:pass");
				$stmt->bindParam(':log', $login, PDO::PARAM_STR,50);
				$stmt->bindParam(':pass', $pass, PDO::PARAM_STR,50);
				$stmt->execute();
								
				if($row = $stmt->fetch ()){
						$result=true;
						$_SESSION['user_id'] = $row[0];
						$_SESSION['user_login'] = $row[1];
				}
				
				$stmt->closeCursor();
				unset($stmt);
				
			}
			catch(PDOException $e){
				echo $e->getMessage();
			}
		
			return $result;
		}
		
		function getZadania($kat){
			$zadania = array();
			try{
				$stmt = $this->db->prepare("SELECT * FROM zad WHERE id_kat=:kat");
				$stmt->bindParam(':kat', $kat, PDO::PARAM_INT);
				$stmt->execute();
								
				while($row = $stmt->fetch ()){
					$zadania[] = new Zad($row[0],$row[1], $row[2], $row[3], $row[4]);
				}
				
				$stmt->closeCursor();
				unset($stmt);
				
			}
			catch(PDOException $e){
				echo $e->getMessage();
			}

			return $zadania;
		}
		
		function getKategoria($kat){
			$kategoria;
			try{
				$stmt = $this->db->prepare("SELECT nazwa FROM kat WHERE id_kat=:kat");
				$stmt->bindParam(':kat', $kat, PDO::PARAM_INT);
				$stmt->execute();
								
				if($row = $stmt->fetch ()){
					$kategoria = $row[0];
				}
				else
					$kategoria = "Błędne id kategorii";
				
				$stmt->closeCursor();
				unset($stmt);
				
			}
			catch(PDOException $e){
				echo $e->getMessage();
			}

			return $kategoria;
		}
	
		
	function getPrzedmioty(){
		$przedmioty = array();
		try{
			$stmt = $this->db->prepare("SELECT * from kat");
			$stmt->execute();
			
			while($row = $stmt->fetch ()){
				$przedmioty[] = new Przedmioty($row[0],$row[1],$row[2]);
			}
			
			$stmt->closeCursor();
			unset($stmt);
		}
		catch(PDOException $e){
			echo $e->getMessage();
		}
		
		return $przedmioty;
	}
	
	function deleteZadanie($id){
		$result = 0;
		try{
			$stmt = $this->db->prepare("DELETE FROM zad WHERE id_zad=:id");
			$stmt->bindParam(':id', $id, PDO::PARAM_INT);
			$stmt->execute();
			$result = $stmt->rowCount();
			unset($stmt);
		}
		catch(PDOException $e){
			echo $e->getMessage();
		}
		
		return $result;
	}
	
	function insertZadanie($nazwa, $id_kat, $plik, $trudnosc){
		$result = 0;
		try{
			$nazwa = str_replace(array("-",'"',"'","/","\\"),"",$nazwa);
			$stmt = $this->db->prepare("INSERT INTO zad(id_kat,nazwa,plik_pdf,trudnosc) values(:id_kat,:nazwa,:plik,:trudnosc)");
			$stmt->bindParam(':id_kat', $id_kat, PDO::PARAM_INT);
			$stmt->bindParam(':nazwa', $nazwa, PDO::PARAM_STR,50);
			$stmt->bindParam(':plik', $plik, PDO::PARAM_STR,50);
			$stmt->bindParam(':trudnosc', $trudnosc, PDO::PARAM_INT);
			$stmt->execute();
			$result = $stmt->rowCount();
			unset($stmt);
		}
		catch(PDOException $e){
			echo $e->getMessage();
		}
		
		return $result;
	}
	
	function checkPDF($name){
		$result = 0;
		try{
			$stmt = $this->db->prepare("SELECT id_zad FROM zad WHERE plik_pdf like :plik");
			$name = '%'.$name.'%';	
			$stmt->bindParam(':plik', $name, PDO::PARAM_STR,50);
			$stmt->execute();
			$result = $stmt->rowCount();
			$stmt->closeCursor();
			unset($stmt);
		}
		catch(PDOException $e){
			echo $e->getMessage();
		}
		
		return $result;
	}
	function getZadanie($id){
		$result = null;
		try{
			$stmt = $this->db->prepare("SELECT * FROM zad WHERE id_zad=:id");
			$stmt->bindParam(':id', $id, PDO::PARAM_INT);
			$stmt->execute();
			if($row = $stmt->fetch ())
					$result = new Zad($row[0],$row[1], $row[2], $row[3], $row[4]);
					
			$stmt->closeCursor();
			unset($stmt);
		}
		catch(PDOException $e){
			echo $e->getMessage();
		}
		
		return $result;
	}
	
	function updateZadanie($id, $nazwa, $kat, $rate, $file){
		$result = 0;
		try{
			$nazwa = str_replace(array("-",'"',"'","/","\\"),"",$nazwa);
			$stmt = $this->db->prepare("UPDATE zad SET nazwa=:nazwa, id_kat=:kat, trudnosc=:rate, plik_pdf=:file WHERE id_zad=:id");
			$stmt->bindParam(':id', $id, PDO::PARAM_INT);
			$stmt->bindParam(':kat', $kat, PDO::PARAM_INT);
			$stmt->bindParam(':nazwa', $nazwa, PDO::PARAM_STR,50);
			$stmt->bindParam(':file', $file, PDO::PARAM_STR,50);
			$stmt->bindParam(':rate', $rate, PDO::PARAM_INT);
			$stmt->execute();
			$result = $stmt->rowCount();
			$stmt->closeCursor();
			unset($stmt);
		}
		catch(PDOException $e){
			echo $e->getMessage();
		}
		
		return $result;
	}
	
	function modifyKat($id, $name){
		$result = 0;
		try{
			$name = str_replace(array("-",'"',"'","/","\\"),"",$name);
			$stmt = $this->db->prepare("update kat set nazwa=:name where id_kat=:id");
			$stmt->bindParam(':name', $name, PDO::PARAM_STR,50);
			$stmt->bindParam(':id', $id, PDO::PARAM_INT);
			$stmt->execute();
			$result = $stmt->rowCount();
			unset($stmt);
			
		}
		catch(PDOException $e){
			echo $e->getMessage();
		}
		return $result;
	}
	
	function moveKat($id, $parent){
		$result = 0;
		try{
			$stmt = $this->db->prepare("update kat set id_nadkat=:parent where id_kat=:id");
			$stmt->bindParam(':id', $id, PDO::PARAM_INT);
			$stmt->bindParam(':parent', $parent, PDO::PARAM_INT);
			$stmt->execute();
			$result = $stmt->rowCount();
			unset($stmt);
			
		}
		catch(PDOException $e){
			echo $e->getMessage();
		}
		return $result;
	}
	
	function insertKat($parent, $name){
		$result = 0;
		try{
			if($parent == 0)
				$parent = null;
			$name = str_replace(array("-",'"',"'","/","\\"),"",$name);
			$stmt = $this->db->prepare("insert into kat(nazwa, id_nadkat) values (:name,:parent)");
			$stmt->bindParam(':name', $name, PDO::PARAM_STR,50);
			$stmt->bindParam(':parent', $parent, PDO::PARAM_INT);
			$stmt->execute();
			$result = $stmt->rowCount();
			unset($stmt);
			
		}
		catch(PDOException $e){
			echo $e->getMessage();
		}
		return $result;
	}
	
	function deleteKat($id){
		$result = 0;
		$toDel = array();
		$przedmioty = $this->getPrzedmioty();
		foreach ($przedmioty as &$p)
			if($p->id_parent == $id)
				$toDel[] = $p->id;
		
		foreach ($toDel as &$p){
			$this->deleteKat($p);
		}
		
		try{
			$stmt = $this->db->prepare("DELETE FROM kat WHERE id_kat=:id");
			$stmt->bindParam(':id', $id, PDO::PARAM_INT);
			$stmt->execute();
			$result = $stmt->rowCount();
			unset($stmt);
			
		}
		catch(PDOException $e){
			echo $e->getMessage();
		}
		return $result;
	}
}
?>

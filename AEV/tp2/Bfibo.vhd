----------------------------------------------------------------------------------
-- Copyright : UNIVERSITE DE LILLE 1 - INRIA Lille Nord de France
--  Villeneuve d'Accsq France
-- 
-- Module Name  : IP FIBO generator
-- Project Name :  Homade V2.1
-- Revision :      no
--                                         
-- Target Device :     spartan 6 spartan 3
-- Tool Version : tested on ISE 12.4,
--                                                   
-- Description :  IP FIBO generator
-- 
-- 
-- Contributor(s) :
-- Dekeyser Jean-Luc ( Creation  juin 2012) jean-luc.dekeyser@lifl.fr
-- Wissem Chouchene ( revision 001,  Ocotbre 2012) wissem.chouchene@inria.fr
-- 
-- 
-- Cecil Licence:
-- This software is a computer program whose purpose is to Implement the
-- Homade processor on Xilinx FPGA systems.
-- 
-- This software is governed by the CeCILL license under French law and
-- abiding by the rules of distribution of free software.  You can  use,
-- modify and/ or redistribute the software under the terms of the CeCILL
-- license as circulated by CEA, CNRS and INRIA at the following URL
-- "http://www.cecill.info".
-- 
-- As a counterpart to the access to the source code and  rights to copy,
-- modify and redistribute granted by the license, users are provided only
-- with a limited warranty  and the software's author,  the holder of the
-- economic rights,  and the successive licensors  have only  limited
-- liability.
--                                                                                          
-- In this respect, the user's attention is drawn to the risks associated
-- with loading,  using,  modifying and/or developing or reproducing the
-- software by the user in light of its specific status of free software,
-- that may mean  that it is complicated to manipulate,  and  that  also
-- therefore means  that it is reserved for developers  and  experienced
-- professionals having in-depth computer knowledge. Users are therefore
-- encouraged to load and test the software's suitability as regards their                                                                           
-- requirements in conditions enabling the security of their systems and/or
-- data to be ensured and,  more generally, to use and operate it in the
-- same conditions as regards security.
-- 
-- The fact that you are presently reading this means that you have had
-- knowledge of the CeCILL license and that you accept its terms.   
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all; 


entity fibogen is
    Port ( clk : in  STD_LOGIC;
           init : in  STD_LOGIC;
           fiboout : out  STD_LOGIC_VECTOR (31 downto 0));
	
	attribute clock_signal : string;
	attribute clock_signal of clk : signal is "yes";
end fibogen;

architecture Behavioral of fibogen is

 signal fib, fib2 : std_logic_vector(31 downto 0);
      
		
begin     
process  (clk, init)      
begin             
	if clk'event and clk='1' then
		if init ='1' then
			fib <=x"00000000";
			fib2<= x"00000001";  
		else 
			fib2 <= fib2 + fib;
			fib <= fib2;
		end if;
		end if;
end process;                                                                                                                                                                                                                            
fiboout<= fib2;

end Behavioral;


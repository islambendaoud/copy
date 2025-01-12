-- file: dcm1_tb.vhd
-- 
-- (c) Copyright 2008 - 2010 Xilinx, Inc. All rights reserved.
-- 
-- This file contains confidential and proprietary information
-- of Xilinx, Inc. and is protected under U.S. and
-- international copyright and other intellectual property
-- laws.
-- 
-- DISCLAIMER
-- This disclaimer is not a license and does not grant any
-- rights to the materials distributed herewith. Except as
-- otherwise provided in a valid license issued to you by
-- Xilinx, and to the maximum extent permitted by applicable
-- law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
-- WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
-- AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
-- BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
-- INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
-- (2) Xilinx shall not be liable (whether in contract or tort,
-- including negligence, or under any other theory of
-- liability) for any loss or damage of any kind or nature
-- related to, arising under or in connection with these
-- materials, including for any direct, or any indirect,
-- special, incidental, or consequential loss or damage
-- (including loss of data, profits, goodwill, or any type of
-- loss or damage suffered as a result of any action brought
-- by a third party) even if such damage or loss was
-- reasonably foreseeable or Xilinx had been advised of the
-- possibility of the same.
-- 
-- CRITICAL APPLICATIONS
-- Xilinx products are not designed or intended to be fail-
-- safe, or for use in any application requiring fail-safe
-- performance, such as life-support or safety devices or
-- systems, Class III medical devices, nuclear facilities,
-- applications related to the deployment of airbags, or any
-- other applications that could lead to death, personal
-- injury, or severe property or environmental damage
-- (individually and collectively, "Critical
-- Applications"). Customer assumes the sole risk and
-- liability of any use of Xilinx products in Critical
-- Applications, subject only to applicable laws and
-- regulations governing limitations on product liability.
-- 
-- THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
-- PART OF THIS FILE AT ALL TIMES.
-- 

------------------------------------------------------------------------------
-- Clocking wizard demonstration testbench
------------------------------------------------------------------------------
-- This demonstration testbench instantiates the example design for the 
--   clocking wizard. Input clocks are toggled, which cause the clocking
--   network to lock and the counters to increment.
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library work;
use work.all;

entity dcm1_tb is
end dcm1_tb;

architecture test of dcm1_tb is

  -- Clock to Q delay of 100 ps
  constant TCQ               : time := 100 ps;
  -- timescale is 1ps
  constant ONE_NS      : time := 1 ns;
  -- how many cycles to run
  constant COUNT_PHASE : integer := 1024 + 1;


  -- we'll be using the period in many locations
  constant PER1        : time := 10.0 ns;


  -- Declare the input clock signals
  signal CLK_IN1       : std_logic := '1';
  -- The high bit of the sampling counter
  signal COUNT         : std_logic;
  -- Status and control signals
  signal RESET         : std_logic := '0';
  signal COUNTER_RESET : std_logic := '0';

component dcm1_exdes
generic (
  TCQ               : in time := 100 ps);
port
 (-- Clock in ports
  CLK_IN1           : in  std_logic;
  -- Reset that only drives logic in example design
  COUNTER_RESET     : in  std_logic;
  -- High bits of counters driven by clocks
  COUNT             : out std_logic;
  -- Status and control signals
  RESET             : in  std_logic
 );
end component;

begin

  -- Input clock generation
  --------------------------------------
  process begin
    CLK_IN1 <= not CLK_IN1; wait for (PER1/2);
  end process;

  -- Test sequence
  process begin
    RESET      <= '1';
    wait for (PER1*6);
    RESET      <= '0';
    -- can't probe into hierarchy, wait "some time" for lock
    wait for (PER1*20);
    COUNTER_RESET <= '1';
    wait for (PER1*20);
    COUNTER_RESET <= '0';
    wait for (PER1*COUNT_PHASE);

    report "Simulation Stopped." severity failure;
    wait;
  end process;

  -- Instantiation of the example design containing the clock
  --    network and sampling counters
  -----------------------------------------------------------
  dut : dcm1_exdes
  generic map (
    TCQ                => TCQ)
  port map
   (-- Clock in ports
    CLK_IN1            => CLK_IN1,
    -- Reset for logic in example design
    COUNTER_RESET      => COUNTER_RESET,
    -- High bits of the counters
    COUNT              => COUNT,
    -- Status and control signals
    RESET              => RESET);

end test;

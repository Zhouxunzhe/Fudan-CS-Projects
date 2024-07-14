--
-- PostgreSQL database dump
--

-- Dumped from database version 14.2
-- Dumped by pg_dump version 14.2

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: building; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.building (
    ubid integer NOT NULL,
    sid character varying(255) NOT NULL,
    bid character varying(255) NOT NULL
);


ALTER TABLE public.building OWNER TO postgres;

--
-- Name: building_ubid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.building_ubid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.building_ubid_seq OWNER TO postgres;

--
-- Name: building_ubid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.building_ubid_seq OWNED BY public.building.ubid;


--
-- Name: inspection; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.inspection (
    uiid integer NOT NULL,
    sid character varying(255) NOT NULL,
    iid character varying(255) NOT NULL,
    inspect_date character varying(255) NOT NULL,
    is_inspected character varying(255) NOT NULL,
    result character varying(255) NOT NULL
);


ALTER TABLE public.inspection OWNER TO postgres;

--
-- Name: inspection_uiid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.inspection_uiid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.inspection_uiid_seq OWNER TO postgres;

--
-- Name: inspection_uiid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.inspection_uiid_seq OWNED BY public.inspection.uiid;


--
-- Name: student; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.student (
    sid character varying(255) NOT NULL,
    name character varying(255) NOT NULL,
    password character varying(255) NOT NULL,
    is_superuser boolean NOT NULL,
    major character varying(255) NOT NULL,
    phone character varying(255) NOT NULL,
    situation character varying(255) NOT NULL,
    is_active boolean NOT NULL
);


ALTER TABLE public.student OWNER TO postgres;

--
-- Data for Name: student; Type: TABLE DATA; Schema: public; Owner: postgres
--
COPY public.student (sid, name, password, is_superuser, major, phone, situation, is_active) FROM stdin;
20307110009     zhouxunzhe      gqJlmrEzvHL5zt3t$eC69/WT6xmSYPZ5ph59YjcLu+sPdqONk+X8r9pZk9UE=   f       SS      18012341234     negative        t
20307110021     zhouxunzhe      y9sS4TuCGbWa85rp$HtiZPCRh4q8Ef/qz2xVds6z2z/fOQ6IOg+hyY1ClEaQ=   f       SS      18012341234     negative        t
20307110016     zhouxunzhe      BHSXBM1tecHB07QD$VkP6HwQkjMOp59LLOiE6w9BPNFegOldTGAEueLyFx3o=   f       CS      18012341234     negative        t
20307110006     zhouxunzhe      TdRsx4aJlpEO0B5p$H+EDUtLm4EqChtGmjyV2MPBUFxnq08W3uJ4YRkEJOy4=   f       CS      18012341234     negative        t
20307110005     zhouxunzhe      kLh2EQEwKtGjCEkE$RSSP6Ib4QVLyYjb5rkzA4kIlm6ZLVkG5/ryqPo89lKs=   f       CS      18012341234     positive        t
20307110017     zhouxunzhe      2MmqamHxLMDceRww$xxizIRS5NQr8pdY3R3lpNz9h7Wn1+VsqjpNAzbp2mag=   f       CS      18012341234     positive        t
20307110022     zhouxunzhe      9jKHRMMYeNDBTfZT$av4biH5qLQ3M8igU/A7DXWAFnPNdAITUxorGBaubBq0=   f       CS      18012341234     positive        t
20307110012     zhouxunzhe      JlUZPCBVEdbzmw4X$vkE0yjyxbphSbwOR3mwt2quvb7Qq6SWD5XbwIjY5qpE=   f       CS      18012341234     negative        t
20307110008     zhouxunzhe      IiNzYWGIbKVsUXVf$kWodu7NET6adFFzpBa8N1s5nO0kFzQ9ZkiRrdFJ7AbA=   f       CS      18012341234     negative        t
20307110013     zhouxunzhe      ZkJh5uLY6b1LRRGM$SlVD3im0aVU7V4cYJFD3rKRhImYCIE9zT9Q6U0CZZtY=   f       CS      18012341234     negative        t
20307110020     zhouxunzhe      TT0K7sb7SGuVRahA$XZoWmOhYFWhKSCVyO2wS8jn2J91QCALKnzubexYSXsM=   f       SS      18012341234     negative        t
20307110014     zhouxunzhe      jD5lG8Sd1cytvyuh$P59/rJafWXp2vygbpy+g5AIf9NyY/wZj/IVyY+M10Mo=   f       CS      18012341234     negative        t
20307110015     zhouxunzhe      yPpLngaS64tiZcR0$1Za39hbUC449Xqw+FOYF+fW2MY9T+DclTrVRWLwLisI=   f       CS      18012341234     negative        t
123123123       admin   dYL4Z2BdNFsPBAIF$KDMtRdAlurO7Sjw1hjQrrCo1dsVQXz/ktNnAJn8o81k=   t       CS      18012341234    negative t
20307110003     zhouxunzhe      exStzxrWcVSYKrqU$IUgt14KuSBxSI8Y4S+Wgu56G+gmJLKrBA6bk4HbHfLo=   f       SS      18012341234     negative        t
20307110001     zhouxunzhe      sySib2Tfp8EY3Hoc$pHXEpEPvlCz9Cwj1O26BV8fBeak4eJHL4GTPCegCdMQ=   f       CS      18012341234     negative        t
20307110002     zhouxunzhe      RwREHi1Bb0LIF2Ve$EYLfZtC2ngJSczQvqAzhpJqaEvJbl9rnj4GqPq/RvbU=   f       CS      18012341234     negative        t
20307110018     zhouxunzhe      PYVPFxsSTPJki3h3$+pDbTPA9PxpD4P25+OmIn5IGVsq4C8zJczJqnz4iShU=   f       CS      18012341234     negative        t
20307110019     zhouxunzhe      l1DLdGAj7B9TMO1d$44tu2ghxUexBdeZBuL1xeFHY4w6RMIAvtK4wYgWfmhE=   f       CS      18012341234     negative        t
\.

--
-- Data for Name: building; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.building (sid, bid) FROM stdin;
20307110008     003
20307110012     001
20307110013     001
20307110015     004
20307110017     006
20307110018     007
20307110019     008
20307110020     009
20307110021     010
20307110009     003
20307110003     011
20307110016     005
20307110002     005
20307110014     012
20307110006     013
20307110022     014
20307110001     001
20307110005     001
123123123       001
\.

--
-- Data for Name: inspection; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.inspection (sid, iid, inspect_date, is_inspected, result) FROM stdin;
20307110005     001     2022/04/03      NOT     NULL
20307110021     001     2022/04/03      NOT     NULL
20307110003     001     2022/04/03      NOT     NULL
20307110001     001     2022/04/02      NOT     NULL
20307110002     001     2022/04/03      NOT     NULL
20307110001     001     2022/04/03      NOT     NULL
\.

--
-- Name: building ubid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.building ALTER COLUMN ubid SET DEFAULT nextval('public.building_ubid_seq'::regclass);


--
-- Name: inspection uiid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.inspection ALTER COLUMN uiid SET DEFAULT nextval('public.inspection_uiid_seq'::regclass);


--
-- Name: building building_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.building
    ADD CONSTRAINT building_pkey PRIMARY KEY (ubid);


--
-- Name: inspection inspection_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.inspection
    ADD CONSTRAINT inspection_pkey PRIMARY KEY (uiid);


--
-- Name: student student_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.student
    ADD CONSTRAINT student_pkey PRIMARY KEY (sid);


--
-- Name: building building_sid_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.building
    ADD CONSTRAINT building_sid_fkey FOREIGN KEY (sid) REFERENCES public.student(sid);


--
-- Name: inspection inspection_sid_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.inspection
    ADD CONSTRAINT inspection_sid_fkey FOREIGN KEY (sid) REFERENCES public.student(sid);


--
-- PostgreSQL database dump complete
--

